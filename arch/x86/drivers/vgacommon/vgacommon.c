#include <arch/x86/drivers/vgacommon/vgacommon.h>

vmode_t *vmode_current;

void vga_init(vmode_t *video_mode, size_t kernel_vbase){
   vmode_current = video_mode;
   vmode_current->kernel_vbase = kernel_vbase;
   vmode_current->vmode_init(vmode_current);
}
