/* desmume.c - this file is part of DeSmuME
 *
 * Copyright (C) 2006,2007 DeSmuME Team
 * Copyright (C) 2007 Pascal Giard (evilynux)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "types.h"
#include "NDSSystem.h"
#include "SPU.h"
#include "sndsdl.h"
#include "ctrlssdl.h"
#include "desmume.h"

volatile BOOL execute = FALSE;
BOOL click = FALSE;
BOOL fini = FALSE;
unsigned long glock = 0;

u8 *desmume_rom_data = NULL;
u32 desmume_last_cycle;

void desmume_init( struct armcpu_memory_iface *arm9_mem_if,
                   struct armcpu_ctrl_iface **arm9_ctrl_iface,
                   struct armcpu_memory_iface *arm7_mem_if,
                   struct armcpu_ctrl_iface **arm7_ctrl_iface,
                   int disable_sound)
{
#ifdef GDB_STUB
	NDS_Init( arm9_mem_if, arm9_ctrl_iface,
                  arm7_mem_if, arm7_ctrl_iface);
#else
        NDS_Init();
#endif
        if ( !disable_sound) {
          SPU_ChangeSoundCore(SNDCORE_SDL, 735 * 4);
        }
	execute = FALSE;
}

void desmume_free( void)
{
	execute = FALSE;
	NDS_DeInit();
}

void desmume_pause( void)
{
	execute = FALSE;
	SPU_Pause(1);
}
void desmume_resume( void)
{
	execute = TRUE;
	SPU_Pause(0);
}
void desmume_toggle( void)
{
	execute = (execute) ? FALSE : TRUE;
}
BOOL desmume_running( void)
{
	return execute;
}

void desmume_cycle( void)
{
  u16 keypad;
  /* Joystick events */
  /* Retrieve old value: can use joysticks w/ another device (from our side) */
  keypad = get_keypad();
  /* Look for queued events */
  process_joystick_events( &keypad);
  /* Update keypad value */
  update_keypad(keypad);

  desmume_last_cycle = NDS_exec<false>((560190 << 1) - desmume_last_cycle);
  SPU_Emulate_user();
  SPU_Emulate_core();
}
 
