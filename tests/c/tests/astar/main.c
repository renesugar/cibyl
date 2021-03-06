/*********************************************************************
 *
 * Copyright (C) 2006,  Blekinge Institute of Technology
 *
 * Filename:      main.c
 * Author:        Simon Kagstrom <ska@bth.se>
 * Description:  A* / Cibyl
 *
 * $Id: main.c 12265 2006-11-20 07:02:49Z ska $
 *
 ********************************************************************/
#ifndef HOST
#include <java/lang.h>
#endif
#include <stdio.h>

#include "finder.h"

#define LEVEL_W_0 48
#define LEVEL_H_0 48

#define TILE_W    64 /* OK, should not be static */

static int level_map_0[] =
{
  0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,3,4,4,0,0,2,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,3,4,4,0,0,2,0,
  4,0,1,4,4,0,0,1,1,1,1,1,1,1,1,1,0,3,4,4,0,0,2,0,4,0,1,4,4,0,0,1,1,1,1,1,1,1,1,1,0,3,4,4,0,0,2,0,
  4,4,1,4,4,0,0,1,4,4,0,0,2,4,0,0,0,3,3,4,0,0,2,0,4,4,1,4,4,0,0,1,4,4,0,0,2,4,0,0,0,3,3,4,0,0,2,0,
  4,0,1,1,1,1,1,1,4,4,4,0,2,4,4,0,0,3,3,4,2,2,2,0,4,0,1,1,1,1,1,1,4,4,4,0,2,4,4,0,0,3,3,4,2,2,2,0,
  0,0,1,0,0,0,0,3,3,4,4,3,2,2,4,0,0,4,3,4,4,0,2,0,0,0,1,0,0,0,0,3,3,4,4,3,2,2,4,0,0,4,3,4,4,0,2,0,
  0,0,1,0,0,3,3,3,3,0,0,3,4,2,4,0,0,0,3,4,4,0,2,0,0,0,1,0,0,3,3,3,3,0,0,3,4,2,4,0,0,0,3,4,4,0,2,0,
  0,0,1,0,0,3,3,3,3,0,0,3,3,2,0,0,0,0,3,3,4,0,2,0,0,0,1,0,0,3,3,3,3,0,0,3,3,2,0,0,0,0,3,3,4,0,2,0,
  0,0,1,0,0,3,3,3,0,0,0,0,0,2,0,0,0,0,4,3,4,0,2,0,0,0,1,0,0,3,3,3,0,0,0,0,0,2,0,0,0,0,4,3,4,0,2,0,
  4,0,1,0,3,3,3,0,0,0,0,0,1,1,1,1,1,1,0,3,4,0,2,0,4,0,1,0,3,3,3,0,0,0,0,0,1,1,1,1,1,1,0,3,4,0,2,0,
  4,4,1,1,1,1,1,1,1,1,1,1,1,0,0,3,0,1,0,3,0,0,2,0,4,4,1,1,1,1,1,1,1,1,1,1,1,0,0,3,0,1,0,3,0,0,2,0,
  4,4,1,0,0,0,0,4,4,2,0,0,0,3,3,3,0,1,0,0,0,0,2,0,4,4,1,0,0,0,0,4,4,2,0,0,0,3,3,3,0,1,0,0,0,0,2,0,
  0,0,1,0,4,0,0,4,0,2,0,0,0,3,3,3,0,1,1,1,1,1,1,1,0,0,1,0,4,0,0,4,0,2,0,0,0,3,3,3,0,1,1,1,1,1,1,1,
  0,0,1,0,0,0,0,0,0,2,0,4,0,0,3,3,0,0,0,2,0,0,4,4,0,0,1,0,0,0,0,0,0,2,0,4,0,0,3,3,0,0,0,2,0,0,4,4,
  0,0,1,0,0,0,0,0,0,2,0,0,0,0,3,3,3,0,0,2,0,4,4,4,0,0,1,0,0,0,0,0,0,2,0,0,0,0,3,3,3,0,0,2,0,4,4,4,
  0,0,1,1,1,1,1,1,1,1,0,0,0,0,3,3,3,4,0,2,4,4,4,4,0,0,1,1,1,1,1,1,1,1,0,0,0,0,3,3,3,4,0,2,4,4,4,4,
  0,0,0,0,3,3,0,0,0,1,0,0,0,0,0,0,3,4,0,2,0,4,4,4,0,0,0,0,3,3,0,0,0,1,0,0,0,0,0,0,3,4,0,2,0,4,4,4,
  0,0,0,0,3,3,3,0,0,1,0,4,0,0,0,0,0,4,0,2,0,0,4,4,0,0,0,0,3,3,3,0,0,1,0,4,0,0,0,0,0,4,0,2,0,0,4,4,
  3,0,0,0,3,3,3,0,0,1,0,4,4,0,0,0,0,0,0,2,0,0,0,0,3,0,0,0,3,3,3,0,0,1,0,4,4,0,0,0,0,0,0,2,0,0,0,0,
  3,3,0,0,0,3,0,0,0,1,0,0,0,0,0,0,0,0,0,2,0,0,0,0,3,3,0,0,0,3,0,0,0,1,0,0,0,0,0,0,0,0,0,2,0,0,0,0,
  3,3,3,0,0,0,0,0,0,1,0,0,0,0,0,2,2,2,2,2,2,2,2,2,3,3,3,0,0,0,0,0,0,1,0,0,0,0,0,2,2,2,2,2,2,2,2,2,
  3,3,3,3,0,0,4,4,0,1,1,2,2,2,2,2,3,0,0,0,0,0,0,0,3,3,3,3,0,0,4,4,0,1,1,2,2,2,2,2,3,0,0,0,0,0,0,0,
  0,0,3,3,0,0,0,4,0,0,1,0,0,0,0,3,3,3,3,0,0,0,0,0,0,0,3,3,0,0,0,4,0,0,1,0,0,0,0,3,3,3,3,0,0,0,0,0,
  0,0,0,3,3,3,0,0,0,0,1,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,1,0,0,0,0,0,3,3,3,0,0,0,0,0,
  4,0,0,0,0,0,0,4,0,0,1,0,4,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,4,0,0,1,0,4,0,0,0,0,0,0,0,0,0,0,0,
  0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,3,4,4,0,0,2,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,3,4,4,0,0,2,0,
  4,0,1,4,4,0,0,1,1,1,1,1,1,1,1,1,0,3,4,4,0,0,2,0,4,0,1,4,4,0,0,1,1,1,1,1,1,1,1,1,0,3,4,4,0,0,2,0,
  4,4,1,4,4,0,0,1,4,4,0,0,2,4,0,0,0,3,3,4,0,0,2,0,4,4,1,4,4,0,0,1,4,4,0,0,2,4,0,0,0,3,3,4,0,0,2,0,
  4,0,1,1,1,1,1,1,4,4,4,0,2,4,4,0,0,3,3,4,2,2,2,0,4,0,1,1,1,1,1,1,4,4,4,0,2,4,4,0,0,3,3,4,2,2,2,0,
  0,0,1,0,0,0,0,3,3,4,4,3,2,2,4,0,0,4,3,4,4,0,2,0,0,0,1,0,0,0,0,3,3,4,4,3,2,2,4,0,0,4,3,4,4,0,2,0,
  0,0,1,0,0,3,3,3,3,0,0,3,4,2,4,0,0,0,3,4,4,0,2,0,0,0,1,0,0,3,3,3,3,0,0,3,4,2,4,0,0,0,3,4,4,0,2,0,
  0,0,1,0,0,3,3,3,3,0,0,3,3,2,0,0,0,0,3,3,4,0,2,0,0,0,1,0,0,3,3,3,3,0,0,3,3,2,0,0,0,0,3,3,4,0,2,0,
  0,0,1,0,0,3,3,3,0,0,0,0,0,2,0,0,0,0,4,3,4,0,2,0,0,0,1,0,0,3,3,3,0,0,0,0,0,2,0,0,0,0,4,3,4,0,2,0,
  4,0,1,0,3,3,3,0,0,0,0,0,1,1,1,1,1,1,0,3,4,0,2,0,4,0,1,0,3,3,3,0,0,0,0,0,1,1,1,1,1,1,0,3,4,0,2,0,
  4,4,1,1,1,1,1,1,1,1,1,1,1,0,0,3,0,1,0,3,0,0,2,0,4,4,1,1,1,1,1,1,1,1,1,1,1,0,0,3,0,1,0,3,0,0,2,0,
  4,4,1,0,0,0,0,4,4,2,0,0,0,3,3,3,0,1,0,0,0,0,2,0,4,4,1,0,0,0,0,4,4,2,0,0,0,3,3,3,0,1,0,0,0,0,2,0,
  0,0,1,0,4,0,0,4,0,2,0,0,0,3,3,3,0,1,1,1,1,1,1,1,0,0,1,0,4,0,0,4,0,2,0,0,0,3,3,3,0,1,1,1,1,1,1,1,
  0,0,1,0,0,0,0,0,0,2,0,4,0,0,3,3,0,0,0,2,0,0,4,4,0,0,1,0,0,0,0,0,0,2,0,4,0,0,3,3,0,0,0,2,0,0,4,4,
  0,0,1,0,0,0,0,0,0,2,0,0,0,0,3,3,3,0,0,2,0,4,4,4,0,0,1,0,0,0,0,0,0,2,0,0,0,0,3,3,3,0,0,2,0,4,4,4,
  0,0,1,1,1,1,1,1,1,1,0,0,0,0,3,3,3,4,0,2,4,4,4,4,0,0,1,1,1,1,1,1,1,1,0,0,0,0,3,3,3,4,0,2,4,4,4,4,
  0,0,0,0,3,3,0,0,0,1,0,0,0,0,0,0,3,4,0,2,0,4,4,4,0,0,0,0,3,3,0,0,0,1,0,0,0,0,0,0,3,4,0,2,0,4,4,4,
  0,0,0,0,3,3,3,0,0,1,0,4,0,0,0,0,0,4,0,2,0,0,4,4,0,0,0,0,3,3,3,0,0,1,0,4,0,0,0,0,0,4,0,2,0,0,4,4,
  3,0,0,0,3,3,3,0,0,1,0,4,4,0,0,0,0,0,0,2,0,0,0,0,3,0,0,0,3,3,3,0,0,1,0,4,4,0,0,0,0,0,0,2,0,0,0,0,
  3,3,0,0,0,3,0,0,0,1,0,0,0,0,0,0,0,0,0,2,0,0,0,0,3,3,0,0,0,3,0,0,0,1,0,0,0,0,0,0,0,0,0,2,0,0,0,0,
  3,3,3,0,0,0,0,0,0,1,0,0,0,0,0,2,2,2,2,2,2,2,2,2,3,3,3,0,0,0,0,0,0,1,0,0,0,0,0,2,2,2,2,2,2,2,2,2,
  3,3,3,3,0,0,4,4,0,1,1,2,2,2,2,2,3,0,0,0,0,0,0,0,3,3,3,3,0,0,4,4,0,1,1,2,2,2,2,2,3,0,0,0,0,0,0,0,
  0,0,3,3,0,0,0,4,0,0,1,0,0,0,0,3,3,3,3,0,0,0,0,0,0,0,3,3,0,0,0,4,0,0,1,0,0,0,0,3,3,3,3,0,0,0,0,0,
  0,0,0,3,3,3,0,0,0,0,1,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,1,0,0,0,0,0,3,3,3,0,0,0,0,0,
  4,0,0,0,0,0,0,4,0,0,1,0,4,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,4,0,0,1,0,4,0,0,0,0,0,0,0,0,0,0,0,
};

int tile_cost_vector[] =
{
   3, /* Grass */
   1, /* Tarmac */
   2, /* Dirt road */
  10, /* Mountain */
   8, /* Forest */
};

static char nibble_to_hex(int nibble)
{
  int val = nibble;

  if (val > 9)
    return (char)(val + 'a' - 10);
  return (char)(val + '0');
}

void int_to_hex(int val)
{
  char out[9];

  memset(out, 0, 9);

  out[7] = nibble_to_hex(val & 0xf);
  out[6] = nibble_to_hex((val & 0xf0) >> 4);
  out[5] = nibble_to_hex((val & 0xf00) >> 8);
  out[4] = nibble_to_hex((val & 0xf000) >> 12);
  out[3] = nibble_to_hex((val & 0xf0000) >> 16);
  out[2] = nibble_to_hex((val & 0xf00000) >> 20);
  out[1] = nibble_to_hex((val & 0xf000000) >> 24);
  out[0] = nibble_to_hex((val & 0xf0000000) >> 28);

  puts(out);
}

extern void pf_init(finder_t *p_finder);
extern void pf_fini(finder_t *p_finder);
extern int checks;

#include <test.h>
int astar_main(int argc, char *argv[])
{
  finder_t finder;
  finder_path_t *path;
  finder_path_elem_t *p;
  int before, after;

  if (finder_init(&finder,
		  level_map_0,
		  LEVEL_W_0, LEVEL_H_0,
		  NULL,
		  tile_cost_vector) < 0)
    return 1;

  pf_init(&finder);
#ifndef HOST
  before = NOPH_System_currentTimeMillis();
#endif
  path = a_star_implementation(&finder,
		   1, 1,
		   47, 47);
#ifndef HOST
  after = NOPH_System_currentTimeMillis();
  test_output("Cibyl astar time: %d ms\n", (after - before));
#endif

  if (path == NULL)
    return 1;

  for (p = path->p_first;
       p;
       p = p->p_next)
    {
    }

  return 0;
}
