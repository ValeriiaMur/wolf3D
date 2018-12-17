#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "./libft/libft.h"
#include "./minilibx/mlx.h"
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

#define screenWidth 800
#define screenHeight 800
#define mapWidth 24
#define mapHeight 24

typedef struct	s_data
{
	void		*mlx_ptr;
	void		*win_ptr;
}				t_data;

typedef struct s_color
{
	int RGB_Red;
	int RGB_Green;
	int RGB_Blue;
	int RGB_White;
	int RGB_Yellow;
}				t_color;

typedef struct s_game
{
	double posX;
	double posY;
  double dirX;
  double dirY; //initial direction vector
  double planeX;
  double planeY; //the 2d raycaster version of camera plane
  double cameraX; //x-coordinate in camera space
  double rayDirX;
  double rayDirY;
  int MapX;
  int MapY;
  double sideDistX;
      double sideDistY;
	  double deltaDistX;
	 double deltaDistY;
	 double perpWallDist;
	 int stepX;
	 int stepY;
	 int hit;
	 int side;
	 int worldMap[mapWidth][mapHeight];
	 int lineHeight;
	 int drawStart;
	 int drawEnd;
	 double frameTime;
 	double moveSpeed;
}			t_game;

int		keys(int key, t_data *data, t_game *game);
int wolf(t_data *data, t_game *game);
void init(t_data *data, t_game *game);
void drawline(int x1, int y0, int y1, t_data *data, int  color);


int	main(int argc, char **argv)
{
	t_data *data;
	int		fd;
	char	*line;
	t_game *game;

	fd = 0;
	line = NULL;
	if (argc == 2)
	{
		data = (t_data*)malloc(sizeof(t_data));
		game = (t_game*)malloc(sizeof(t_game));
		data->mlx_ptr = mlx_init();
		data->win_ptr = mlx_new_window(data->mlx_ptr, screenWidth, screenHeight, "Game");
	//	read_map(argv[1], data, fd, line);
	//	mlx_string_put(data->mlx_ptr, data->win_ptr, 400, 100, 0xe6e6fa,
	//		"Wolf3D");
		init(data, game);
		mlx_key_hook(data->win_ptr, keys, data);
		mlx_loop(data->mlx_ptr);
	}
	else if (argc < 2)
		ft_putendl("Please specify the name of the map");
	else
		return (-1);
}

void init(t_data *data, t_game *game)
{
	game->posX = 22;
	game->posY = 12;
  	game->dirX = -1;
	game->dirY = 0;
 	game->planeX = 0;
	game->planeY = 0.66;
	wolf(data, game);
}

int wolf(t_data *data, t_game *game)
{
	clock_t start_t, end_t, total_t;

	int worldMap[mapWidth][mapHeight]=
	{
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	  {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	start_t = clock();
	for (int x = 0; x < screenWidth; x++)
  	{
		game->cameraX = 2 * x / (double)screenWidth - 1; //x-coordinate in camera space
		game->rayDirX = game->dirX + game->planeX * game->cameraX;
		game->rayDirY = game->dirY + game->planeY * game->cameraX;
      	game->MapX = (int)game->posX;
      	game->MapY = (int)game->posY;
      	game->deltaDistX = fabs(1 / game->rayDirX);
      	game->deltaDistY = fabs(1 / game->rayDirY);
      	game->hit = 0; //was there a wall hit?
		if (game->rayDirX < 0)
      	{
        	game->stepX = -1;
        	game->sideDistX = (game->posX - game->MapX) * game->deltaDistX;
      	}
      	else
      	{
        	game->stepX = 1;
        	game->sideDistX = (game->MapX + 1.0 - game->posX) * game->deltaDistX;
      	}
      	if (game->rayDirY < 0)
      	{
        	game->stepY = -1;
        	game->sideDistY = (game->posY - game->MapY) * game->deltaDistY;
      	}
      	else
      	{
        	game->stepY = 1;
        	game->sideDistY = (game->MapY + 1.0 - game->posY) * game->deltaDistY;
      	}
	  	while (game->hit == 0)
	  	{
		  //go to the next square either in X or Y direction
		  if (game->sideDistX < game->sideDistY)
		  {
			  game->sideDistX += game->deltaDistX;
			  game->MapX+= game->stepX;
			  game->side =0;
		  }
		  else
		  {
			  game->sideDistY += game->deltaDistY;
			  game->MapY += game->stepY;
			  game->side = 1;
		  }
		  if (game->worldMap[game->MapX][game->MapY] > 0)
		  {
			  game->hit = 1;
		  }
		}
		if (game->side == 0)
		{
			game->perpWallDist = (game->MapX - game->posX + (1 - game->stepX) / 2) / game->rayDirX;
		}
		else
		{
			game->perpWallDist = (game->MapY - game->posY + (1 - game->stepY) / 2) / game->rayDirY;
		}
		game->lineHeight = screenHeight / game->perpWallDist;
	    game->drawStart = -(game->lineHeight) / 2 + screenHeight / 2;
	    if(game->drawStart < 0)
		{
			game->drawStart = 0;
		}
	    game->drawEnd = game->lineHeight / 2 + screenHeight / 2;
	    if(game->drawEnd >= screenHeight)
		{
			game->drawEnd = screenHeight - 1;
		}
		//choose wall color
	 	t_color color;
	 	int COLOR;
	 	switch(worldMap[game->MapX][game->MapY])
	 	{
	   		case 1:  COLOR = color.RGB_Red;  break; //red
	   		case 2:  COLOR = color.RGB_Green;  break; //green
	   		case 3:  COLOR = color.RGB_Blue;   break; //blue
	   		case 4:  COLOR = color.RGB_White;  break; //white
	   		default: COLOR = color.RGB_Yellow; break; //yellow
	 	}
	 if (game->side == 1)
		 COLOR = COLOR / 2;
	 //draw the pixels of the stripe as a vertical line. Drawstart is
	 //the lowest pixel y and end is the highest y. x0 and x1 is the same
	drawline(x, game->drawStart, game->drawEnd, data, COLOR);
	}
	end_t = clock();
   	game->frameTime = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	game->moveSpeed = game->frameTime * 5.0; //the constant value is in squares/second
	return (0);
}

void drawline(int x1, int y0, int y1, t_data *data, int color)
{
	int x0 = x1;
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = x1 >= x0 ? 1 : -1;
  int sy = y1 >= y0 ? 1 : -1;

  if (dy <= dx)
  {
    int d = (dy << 1) - dx;
    int d1 = dy << 1;
    int d2 = (dy - dx) << 1;
	mlx_pixel_put(data->mlx_ptr, data->win_ptr, x0, y0, color);
    for(int x = x0 + sx, y = y0, i = 1; i <= dx; i++, x += sx)
    {
      if ( d >0)
      {
        d += d2;
        y += sy;
      }
      else
        d += d1;
      mlx_pixel_put(data->mlx_ptr, data->win_ptr, x, y, color);
    }
  }
  else
  {
    int d = (dx << 1) - dy;
    int d1 = dx << 1;
    int d2 = (dx - dy) << 1;
    mlx_pixel_put(data->mlx_ptr, data->win_ptr, x0, y0, color);
    for(int y = y0 + sy, x = x0, i = 1; i <= dy; i++, y += sy)
    {
      if ( d >0)
      {
        d += d2;
        x += sx;
      }
      else
        d += d1;
      mlx_pixel_put(data->mlx_ptr, data->win_ptr, x, y, color);
    }
  }
}

int		keys(int key, t_data *data, t_game *game)
{
	if (key == 53)
		exit(1);
	else if (key == 126)
	{
      if(game->worldMap[(int)(game->posX + game->dirX * game->moveSpeed)][(int)game->posY] == false)
	  {
		  game->posX += game->dirX * game->moveSpeed;
		  printf("this is first case\n");
	  }
	  else if(game->worldMap[(int)game->posX][(int)(game->posY + game->dirY * game->moveSpeed)] == false)
	  {
		  game->posY += game->dirY * game->moveSpeed;
		  printf("esle shit\n");
	  }
	}
	mlx_clear_window(data->mlx_ptr, data->win_ptr);
	wolf(data, game);
	return (0);
}