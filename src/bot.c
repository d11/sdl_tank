////// BOTS //////
#include "config.h"
#include "bot.h"

////
// keep something within display bounds
int constrain_x(int x, int margin)
{
   return(max(min(x, DISPLAY_WIDTH - margin), margin));
}

int constrain_y(int y, int margin)
{
   return(max(min(y, DISPLAY_HEIGHT - margin), margin));
}

// find the next unused bot number
int get_free_bot(Bot *bots)
{
   int i = 0;
   while(bots[i].ex == 1)
   {
      i++;
      if (i>=BOT_MAX)
      {
         printf("Error, cannot create new bots\n");
         i = 0;
         bots[i].ex = 0;
      }
   }
   return(i);
}

// create another bot
void spawn_bot(Bot *bots, int x, int y, float angle)
{
   int b;
   b = get_free_bot(bots);
   bots[b].ex = 1;
   bots[b].x = x;
   bots[b].y = y;
   bots[b].vel.x = 0;
   bots[b].vel.y = 0;
   bots[b].xpos = (float)x;
   bots[b].ypos = (float)y;
   bots[b].angle = angle;

   bots[b].t = 1.0;
   bots[b].hp = 20;
}

void draw_bot(SDL_Surface *screen, Bot *b)
{
   if (b->hp < 5)
   {
      // injured
      put_circle(screen, b->x, b->y, 6, 0x0066AA);
      put_circle(screen, b->x, b->y, 5, 0x004444);
   }
   else
   {
      // healthy
      put_circle(screen, b->x, b->y, 6, 0x0066AA);
      put_circle(screen, b->x, b->y, 5, 0x00FFFF);
   }
   
   // draw bot health bar
   int y, x_start;
   x_start = b->x - 10;
   for (y = b->y - 10; y < b->y - 8; y++)
   {
      put_line(screen, x_start,         y,   x_start + b->hp, y,   HEALTH_COLOR_GOOD);
      put_line(screen, x_start + b->hp, y,   b->x + 10,       y,   HEALTH_COLOR_BAD);
      put_line(screen, x_start,         y+1, x_start + b->hp, y+1, HEALTH_COLOR_GOOD);
      put_line(screen, x_start + b->hp, y+1, b->x + 10,       y+1, HEALTH_COLOR_BAD);
   }
}

void draw_bots(SDL_Surface *screen, Bot *bots)
{
   int i;
   for(i=0; i<BOT_MAX; i++)
   {
      if (bots[i].ex == 1)
      {
         draw_bot(screen, &bots[i]);
      }
   }
}

// handle an individual bot
void do_bot(Bot *b, Player *p, Bullet *bullets, Wall *walls, float dt)
{
   // only need to do anything if the bot exists
   if (b->ex == 1)
   {

      // AI here!
      //
      //
   
      int x_immediate_next, y_immediate_next;
      float step;
      step = 0.002*dt;

      // reached end of current path
      if (b->t >= 1.0)
      {
         // work out new path
         b->t = 0.0;

         b->x_start = b->xpos;
         b->y_start = b->ypos;

         // try to get behind player
         b->x_destination = constrain_x((int)(p->exact_pos.x + cos(p->angle+M_PI)*100.0),4);
         b->y_destination = constrain_y((int)(p->exact_pos.y + sin(p->angle+M_PI)*100.0),4);

      /* Wall *w = NULL;
         while (w = wall_blocks_path(walls, i_vector(b->x_start, b->y_start), i_vector(b->x_destination, b->y_destination)))
         {
            b->x_destination = w->end1.x;
            b->y_destination = w->end1.y;
         }
*/

         b->x_enroute2 = constrain_x((int)(p->exact_pos.x + cos(p->angle+(random_binary()*2-1)*(M_PI/2))*150.0),4);
         b->y_enroute2 = constrain_y((int)(p->exact_pos.y + sin(p->angle+(random_binary()*2-1)*(M_PI/2))*150.0),4);
         b->x_enroute1 = constrain_x((int)(p->exact_pos.x + cos(b->angle+M_PI)*50.0),4);
         b->y_enroute1 = constrain_y((int)(p->exact_pos.y + sin(b->angle+M_PI)*50.0),4);
      }

      // temp - show bot paths
      /* 
      put_circle(screen, b->x_destination, b->y_destination, 2, 0xFF6666);
      put_circle(screen, b->x_enroute1, b->y_enroute1, 2, 0xFF6666);
      put_circle(screen, b->x_enroute2, b->y_enroute2, 2, 0xFF6666);
      put_bezier(screen, b->x_start, b->y_start, b->x_enroute1, b->y_enroute1, b->x_enroute2, b->y_enroute2, b->x_destination, b->y_destination, step, 0x999999);
      */
      // 

      x_immediate_next = bezier_coord(b->xpos, b->x_enroute1, b->x_enroute2, b->x_destination, b->t);
      y_immediate_next = bezier_coord(b->ypos, b->y_enroute1, b->y_enroute2, b->y_destination, b->t);
      
      // temp - move bot along bezier
      b->xpos *= 0.99;
      b->ypos *= 0.99;
      b->xpos += x_immediate_next * 0.01;
      b->ypos += y_immediate_next * 0.01;
      b->xpos += b->vel.x;
      b->ypos += b->vel.y;
      b->vel.x *= 0.9;
      b->vel.y *= 0.9;

      b->t += step;
      // 
      

      // sometimes we shoot at the player
      if (rand() < RAND_MAX/40 * dt)
      {
         float angle = atan2(p->exact_pos.y - b->ypos, p->exact_pos.x - b->xpos) + ((float)rand()/(float)RAND_MAX)*0.1 - 0.05;
         spawn_bullet(bullets, b->xpos + cos(angle) * 10, b->ypos + sin(angle) * 10, angle, 8, 0x66FF88);
         
      }

      /*
      if (b->xpos >= DISPLAY_WIDTH)
      {
         b->ex = 0;
         spawn_bot(150,150,0);
      }
      */



      // handle bot death
      if (b->hp <= 0)
      {
         spawn_explosion(bullets, b->xpos, b->ypos, 120, 0x00FFFF);
         b->ex = 0;

         // temp?
         p->kills++;
      }

      // calculate pixel coordinates
      b->x = constrain_x(roundf(b->xpos),4);
      b->y = constrain_y(roundf(b->ypos),4);
   }
}

Boolean bot_hit_by_bullet(Bot *bt, Bullet *bl)
{
   return(sqrt(pow((bl->x1 - bt->xpos), 2) + pow((bl->y1 - bt->ypos), 2)) < 10);
}

void bot_take_bullet(Bot *bt, Bullet *b)
{
   bt->vel.x += b->vx*0.2;
   bt->vel.y += b->vy*0.2;
   // remove health
   bt->hp -= 1;
}

void do_bots(Bot *bots, Player *p, Bullet *bullets, Wall *walls, float dt)
{
   int i;
   for(i=0; i<BOT_MAX; i++)
   {
      do_bot(&bots[i], p, bullets, walls, dt);
   }
}


