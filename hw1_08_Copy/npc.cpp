#include <cstdlib>
#include <cstring>

#include "headers/dungeon.h"
#include "headers/path_finding.h"
#include "headers/npc.h"

#define min(x, y) ({  \
  typeof(x) _x = (x); \
  typeof(y) _y = (y); \
  _x < _y ? _x : _y;  \
})

uint32_t can_see(Dungeon *d, Character *voyeur, Character *exhibitionist)
{

  Position first, second;
  Position del, f;
  int16_t a, b, c, i;

  first.x = voyeur->pos.x;
  first.y = voyeur->pos.y;
  second.x = exhibitionist->pos.x;
  second.y = exhibitionist->pos.y;

  if ((abs(first.x - second.x) > VISUAL_RANGE) || (abs(first.y - second.y) > VISUAL_RANGE))
  {
    return 0;
  }

  if (second.x > first.x)
  {
    del.x = second.x - first.x;
    f.x = 1;
  }
  else
  {
    del.x = first.x - second.x;
    f.x = -1;
  }

  if (second.y > first.y)
  {
    del.y = second.y - first.y;
    f.y = 1;
  }
  else
  {
    del.y = first.y - second.y;
    f.y = -1;
  }

  if (del.x > del.y)
  {
    a = del.y + del.y;
    c = a - del.x;
    b = c - del.x;
    for (i = 0; i <= del.x; i++)
    {
      if ((d->map[first.y][first.x] < c_room) && i && (i != del.x))
      {
        return 0;
      }
      /*      mappair(first) = ter_debug;*/
      first.x += f.x;
      if (c < 0)
      {
        c += a;
      }
      else
      {
        c += b;
        first.y += f.y;
      }
    }
    return 1;
  }
  else
  {
    a = del.x + del.x;
    c = a - del.y;
    b = c - del.y;
    for (i = 0; i <= del.y; i++)
    {
      if ((d->map[first.y][first.x] < c_room) && i && (i != del.y))
      {
        return 0;
      }

      first.y += f.y;
      if (c < 0)
      {
        c += a;
      }
      else
      {
        c += b;
        first.x += f.x;
      }
    }
    return 1;
  }

  return 1;
}

void npc_next_pos_rand_tunnel(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{

  union {
    uint32_t i;
    uint8_t a[4];
  } r;

  do
  {
    // *x_offset = 0;
    // *y_offset = 0;
    r.i = rand();

    if (r.a[0] > 85 /* 255 / 3 */)
    {
      if (r.a[0] & 1)
      {
        *y_offset = -1;
      }
      else
      {
        *y_offset = 1;
      }
    }
    if (r.a[1] > 85 /* 255 / 3 */)
    {
      if (r.a[1] & 1)
      {
        *x_offset = -1;
      }
      else
      {
        *x_offset = 1;
      }
    }
  } while (d->hardness[c->pos.y + *y_offset][c->pos.x + *x_offset] == 255);

  return;
}

void npc_next_pos_rand(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{

  union {
    uint32_t i;
    uint8_t a[4];
  } r;

  do
  {
    *x_offset = 0;
    *y_offset = 0;
    r.i = rand();
    if (r.a[0] > 85 /* 255 / 3 */)
    {
      if (r.a[0] & 1)
      {
        *y_offset = -1;
      }
      else
      {
        *y_offset = 1;
      }
    }
    if (r.a[1] > 85 /* 255 / 3 */)
    {
      if (r.a[1] & 1)
      {
        *x_offset = -1;
      }
      else
      {
        *x_offset = 1;
      }
    }
  } while (d->map[c->pos.y + *y_offset][c->pos.x + *x_offset] < c_room);
}

void npc_next_pos_line_of_sight(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{
  Position dir;
  dir.y = d->player.y - c->pos.y;
  dir.x = d->player.x - c->pos.x;

  if (dir.y)
  {
    dir.y /= abs(dir.y);
  }

  if (dir.x)
  {
    dir.x /= abs(dir.x);
  }

  if (mapxy(c->pos.x + dir.x, c->pos.y + dir.y) >= c_room)
  {
    *x_offset += dir.x;
    *y_offset += dir.y;
  }
  else if (mapxy(c->pos.x + dir.x, c->pos.y) >= c_room)
  {
    *x_offset += dir.x;
  }
  else if (mapxy(c->pos.x, c->pos.y + dir.y) >= c_room)
  {
    *y_offset += dir.y;
  }
}

void npc_next_pos_line_of_sight_tunnel(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{
  Position dir;
  dir.y = d->player.y - c->pos.y;
  dir.x = d->player.x - c->pos.x;

  if (dir.y)
  {
    dir.y /= abs(dir.y);
  }

  if (dir.x)
  {
    dir.x /= abs(dir.x);
  }

  *x_offset = dir.x;
  *y_offset = dir.y;
}

void npc_next_pos_gradient(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{
  /* Handles both tunneling and non-tunneling versions */
  uint16_t min_cost;

  if (c->behavior & NPC_TUNNEL)
  {
    min_cost = (d->tunneling_map[c->pos.y - 1][c->pos.x] + (d->hardness[c->pos.y - 1][c->pos.x] / 85));
    *y_offset = -1;
    if ((d->tunneling_map[c->pos.y + 1][c->pos.x] + (d->hardness[c->pos.y + 1][c->pos.x] / 85)) < min_cost)
    {
      min_cost = (d->tunneling_map[c->pos.y + 1][c->pos.x] + (d->hardness[c->pos.y + 1][c->pos.x] / 85));
      *y_offset = 1;
    }
    if ((d->tunneling_map[c->pos.y][c->pos.x + 1] + (d->hardness[c->pos.y][c->pos.x + 1] / 85)) < min_cost)
    {
      min_cost = (d->tunneling_map[c->pos.y][c->pos.x + 1] + (d->hardness[c->pos.y][c->pos.x + 1] / 85));
      *x_offset = 1;
    }
    if ((d->tunneling_map[c->pos.y][c->pos.x - 1] + (d->hardness[c->pos.y][c->pos.x - 1] / 85)) < min_cost)
    {
      min_cost = (d->tunneling_map[c->pos.y][c->pos.x - 1] + (d->hardness[c->pos.y][c->pos.x - 1] / 85));
      *x_offset = -1;
    }
    if ((d->tunneling_map[c->pos.y - 1][c->pos.x + 1] + (d->hardness[c->pos.y - 1][c->pos.x + 1] / 85)) < min_cost)
    {
      min_cost = (d->tunneling_map[c->pos.y - 1][c->pos.x + 1] + (d->hardness[c->pos.y - 1][c->pos.x + 1] / 85));
      *x_offset = 1;
      *y_offset = -1;
    }
    if ((d->tunneling_map[c->pos.y + 1][c->pos.x + 1] + (d->hardness[c->pos.y + 1][c->pos.x + 1] / 85)) < min_cost)
    {
      min_cost = (d->tunneling_map[c->pos.y + 1][c->pos.x + 1] + (d->hardness[c->pos.y + 1][c->pos.x + 1] / 85));
      *x_offset = 1;
      *y_offset = 1;
    }
    if ((d->tunneling_map[c->pos.y - 1][c->pos.x - 1] + (d->hardness[c->pos.y - 1][c->pos.x - 1] / 85)) < min_cost)
    {
      min_cost = (d->tunneling_map[c->pos.y - 1][c->pos.x - 1] + (d->hardness[c->pos.y - 1][c->pos.x - 1] / 85));
      *x_offset = -1;
      *y_offset = -1;
    }
    if ((d->tunneling_map[c->pos.y + 1][c->pos.x - 1] + (d->hardness[c->pos.y + 1][c->pos.x - 1] / 85)) < min_cost)
    {
      min_cost = (d->tunneling_map[c->pos.y + 1][c->pos.x - 1] + (d->hardness[c->pos.y + 1][c->pos.x - 1] / 85));
      *x_offset = -1;
      *y_offset = 1;
    }
  }
  else
  {
    /* Make monsters prefer cardinal directions */
    if (d->non_tunneling_map[c->pos.y - 1][c->pos.x] < d->non_tunneling_map[c->pos.y][c->pos.x])
    {
      *y_offset = -1;
      return;
    }
    if (d->non_tunneling_map[c->pos.y + 1][c->pos.x] < d->non_tunneling_map[c->pos.y][c->pos.x])
    {
      *y_offset = 1;
      return;
    }
    if (d->non_tunneling_map[c->pos.y][c->pos.x + 1] < d->non_tunneling_map[c->pos.y][c->pos.x])
    {
      *x_offset = 1;
      return;
    }
    if (d->non_tunneling_map[c->pos.y][c->pos.x - 1] < d->non_tunneling_map[c->pos.y][c->pos.x])
    {
      *x_offset = -1;
      return;
    }
    if (d->non_tunneling_map[c->pos.y - 1][c->pos.x + 1] < d->non_tunneling_map[c->pos.y][c->pos.x])
    {
      *y_offset = -1;
      *x_offset = 1;
      return;
    }
    if (d->non_tunneling_map[c->pos.y + 1][c->pos.x + 1] < d->non_tunneling_map[c->pos.y][c->pos.x])
    {
      *y_offset = 1;
      *x_offset = 1;
      return;
    }
    if (d->non_tunneling_map[c->pos.y - 1][c->pos.x - 1] < d->non_tunneling_map[c->pos.y][c->pos.x])
    {
      *y_offset = -1;
      *x_offset = -1;
      return;
    }
    if (d->non_tunneling_map[c->pos.y + 1][c->pos.x - 1] < d->non_tunneling_map[c->pos.y][c->pos.x])
    {
      *y_offset = 1;
      *x_offset = -1;
      return;
    }
  }
}

static void npc_next_pos_00(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{
  /* not smart; not telepathic; not tunneling; not erratic */
  if (can_see(d, c, d->characters[d->player.y][d->player.x]))
  {
    c->pc_last_known_pos.y = d->player.y;
    c->pc_last_known_pos.x = d->player.x;
    npc_next_pos_line_of_sight(d, c, x_offset, y_offset);
  }
  else
  {
    npc_next_pos_rand(d, c, x_offset, y_offset);
  }
}

static void npc_next_pos_01(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{
  /*     smart; not telepathic; not tunneling; not erratic */
  if (can_see(d, c, d->characters[d->player.y][d->player.x]))
  {
    c->pc_last_known_pos.y = d->player.y;
    c->pc_last_known_pos.x = d->player.x;
    c->have_seen_pc = 1;
    npc_next_pos_line_of_sight(d, c, x_offset, y_offset);
  }
  else if (c->have_seen_pc)
  {
    npc_next_pos_line_of_sight(d, c, x_offset, y_offset);
  }

  if (c->have_seen_pc && (c->pos.x == c->pc_last_known_pos.x) && (c->pos.y == c->pc_last_known_pos.y))
  {
    c->have_seen_pc = 0;
  }
}

static void npc_next_pos_02(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{
  /* not smart;     telepathic; not tunneling; not erratic */
  c->pc_last_known_pos.y = d->player.y;
  c->pc_last_known_pos.x = d->player.x;
  npc_next_pos_line_of_sight(d, c, x_offset, y_offset);
}

static void npc_next_pos_03(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{
  /*     smart;     telepathic; not tunneling; not erratic */
  npc_next_pos_gradient(d, c, x_offset, y_offset);
}

static void npc_next_pos_04(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{
  /* not smart; not telepathic;     tunneling; not erratic */
  if (can_see(d, c, d->characters[d->player.y][d->player.x]))
  {
    c->pc_last_known_pos.y = d->player.y;
    c->pc_last_known_pos.x = d->player.x;
    npc_next_pos_line_of_sight(d, c, x_offset, y_offset);
  }
  else
  {
    npc_next_pos_rand_tunnel(d, c, x_offset, y_offset);
  }
}

static void npc_next_pos_05(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{
  /*     smart; not telepathic;     tunneling; not erratic */
  if (can_see(d, c, d->characters[d->player.y][d->player.x]))
  {
    c->pc_last_known_pos.y = d->player.y;
    c->pc_last_known_pos.x = d->player.x;
    c->have_seen_pc = 1;
    npc_next_pos_line_of_sight(d, c, x_offset, y_offset);
  }
  else if (c->have_seen_pc)
  {
    npc_next_pos_line_of_sight_tunnel(d, c, x_offset, y_offset);
  }

  if (c->have_seen_pc && (c->pos.x == c->pc_last_known_pos.x) && (c->pos.y == c->pc_last_known_pos.y))
  {
    c->have_seen_pc = 0;
  }
}

static void npc_next_pos_06(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{
  /* not smart;     telepathic;     tunneling; not erratic */
  c->pc_last_known_pos.y = d->player.y;
  c->pc_last_known_pos.x = d->player.x;
  npc_next_pos_line_of_sight_tunnel(d, c, x_offset, y_offset);
}

static void npc_next_pos_07(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{
  /*     smart;     telepathic;     tunneling; not erratic */
  npc_next_pos_gradient(d, c, x_offset, y_offset);
}

static void npc_next_pos_erratic(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset);

void (*npc_move_func[])(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset) = {
    npc_next_pos_00,
    npc_next_pos_01,
    npc_next_pos_02,
    npc_next_pos_03,
    npc_next_pos_04,
    npc_next_pos_05,
    npc_next_pos_06,
    npc_next_pos_07,
    npc_next_pos_erratic,
    npc_next_pos_erratic,
    npc_next_pos_erratic,
    npc_next_pos_erratic,
    npc_next_pos_erratic,
    npc_next_pos_erratic,
    npc_next_pos_erratic,
    npc_next_pos_erratic,
};

static void npc_next_pos_erratic(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{
  /*erratic */
  if (rand() & 1)
  {
    npc_next_pos_rand(d, c, x_offset, y_offset);
  }
  else
  {
    npc_move_func[c->behavior & 0x00000007](d, c, x_offset, y_offset);
  }
}

void npc_next_pos(Dungeon *d, Character *c, int8_t *x_offset, int8_t *y_offset)
{
  npc_move_func[c->behavior & 0x0000000f](d, c, x_offset, y_offset);
}