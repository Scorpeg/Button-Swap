#include <psp2/kernel/modulemgr.h>
#include <psp2/ctrl.h>
#include <taihen.h>

static SceUID g_hooks[3];

static tai_hook_ref_t ref_hook;

static int keys_patched(int port, SceCtrlData *ctrl, int count) {

  int ret;

  if (ref_hook == 0)
     ret = 1;
  else
  {
     ret = TAI_CONTINUE(int, ref_hook, port, ctrl, count);

     if ((ctrl->buttons & 0x6000) && ((ctrl->buttons & 0x6000) != 0x6000))
     ctrl->buttons = ctrl->buttons ^ 0x6000;

  }
  return ret;
}

static tai_hook_ref_t ref_hook2;

static int keys_patched2(int port, SceCtrlData *ctrl, int count) {

  int ret;

  if (ref_hook2 == 0)
     ret = 1;
  else
  {
     ret = TAI_CONTINUE(int, ref_hook2, port, ctrl, count);

     if ((ctrl->buttons & 0x6000) && ((ctrl->buttons & 0x6000) != 0x6000))
     ctrl->buttons = ctrl->buttons ^ 0x6000;

  }
  return ret;
}



void _start() __attribute__ ((weak, alias ("module_start")));

int module_start(SceSize argc, const void *args) {

  ref_hook  = 0;
  ref_hook2 = 0;

  g_hooks[1] = taiHookFunctionImport(&ref_hook, 
                                      TAI_MAIN_MODULE,
                                      TAI_ANY_LIBRARY,
                                      0xA9C3CED6, // sceCtrlPeekBufferPositive
                                      keys_patched);

  g_hooks[2] = taiHookFunctionImport(&ref_hook2, 
                                      TAI_MAIN_MODULE,
                                      TAI_ANY_LIBRARY,
                                      0x15F81E8C, // sceCtrlPeekBufferPositive2
                                      keys_patched2);
  return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {

  // free hooks that didn't fail
  if (g_hooks[1] >= 0) taiHookRelease(g_hooks[1], ref_hook);
  if (g_hooks[2] >= 0) taiHookRelease(g_hooks[2], ref_hook2);

  return SCE_KERNEL_STOP_SUCCESS;
}
