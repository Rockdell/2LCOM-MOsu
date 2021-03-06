#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	mmap_t map;

	lm_init();

	if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL)
		return 1;

	struct reg86u reg86;

	reg86.u.w.ax = 0x4F01; /* VBE get mode info *//* translate the buffer linear address to a far pointer */

	reg86.u.w.es = PB2BASE(map.phys); /* set a segment base */
	reg86.u.w.di = PB2OFF(map.phys); /* set the offset accordingly */
	reg86.u.w.cx = mode;
	reg86.u.b.intno = 0x10;

	if (sys_int86(&reg86) != OK) {
		lm_free(&map);
		return 1;
	}

	*vmi_p = *((vbe_mode_info_t *) map.virtual);

	return 0;
}


