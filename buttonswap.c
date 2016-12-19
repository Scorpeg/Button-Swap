// Button Swap taiHEN plugin
// Copyright (c) 2016 Scorp

#include <psp2/kernel/modulemgr.h>
#include <psp2/ctrl.h>
#include <taihen.h>

static SceUID g_hooks[4];

int invertButtons(int port, tai_hook_ref_t ref_hook, SceCtrlData *ctrl, int count) {
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

static tai_hook_ref_t ref_hook1;
static int keys_patched1(int port, SceCtrlData *ctrl, int count) {
    return invertButtons(port, ref_hook1, ctrl, count);
}   

static tai_hook_ref_t ref_hook2;
static int keys_patched2(int port, SceCtrlData *ctrl, int count) {
    return invertButtons(port, ref_hook2, ctrl, count);
}   

static tai_hook_ref_t ref_hook3;
static int keys_patched3(int port, SceCtrlData *ctrl, int count) {
    return invertButtons(port, ref_hook3, ctrl, count);
}   

static tai_hook_ref_t ref_hook4;
static int keys_patched4(int port, SceCtrlData *ctrl, int count) {
    return invertButtons(port, ref_hook4, ctrl, count);
}   


void _start() __attribute__ ((weak, alias ("module_start")));

int module_start(SceSize argc, const void *args) {

  g_hooks[0] = taiHookFunctionImport(&ref_hook1, 
                                      TAI_MAIN_MODULE,
                                      TAI_ANY_LIBRARY,
                                      0xA9C3CED6, // sceCtrlPeekBufferPositive
                                      keys_patched1);

  g_hooks[1] = taiHookFunctionImport(&ref_hook2, 
                                      TAI_MAIN_MODULE,
                                      TAI_ANY_LIBRARY,
                                      0x15F81E8C, // sceCtrlPeekBufferPositive2
                                      keys_patched2);

  g_hooks[2] = taiHookFunctionImport(&ref_hook3, 
                                      TAI_MAIN_MODULE,
                                      TAI_ANY_LIBRARY,
                                      0x67E7AB83, // sceCtrlReadBufferPositive
                                      keys_patched3);

  g_hooks[3] = taiHookFunctionImport(&ref_hook4, 
                                      TAI_MAIN_MODULE,
                                      TAI_ANY_LIBRARY,
                                      0xC4226A3E, // sceCtrlReadBufferPositive2
                                      keys_patched4);

  return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {

  // free hooks that didn't fail
  if (g_hooks[0] >= 0) taiHookRelease(g_hooks[0], ref_hook1);
  if (g_hooks[1] >= 0) taiHookRelease(g_hooks[1], ref_hook2);
  if (g_hooks[2] >= 0) taiHookRelease(g_hooks[2], ref_hook3);
  if (g_hooks[3] >= 0) taiHookRelease(g_hooks[3], ref_hook4);

  return SCE_KERNEL_STOP_SUCCESS;
}
