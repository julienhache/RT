/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 14:19:36 by yguaye            #+#    #+#             */
/*   Updated: 2018/05/31 15:56:22 by jhache           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <libft_base/io.h>
#include "mlx_defs.h"
#include "rt.h"
#include "ocl_data.h"
#include "move.h"
#include "timer.h"

static int			exit_rt(t_rt *core)
{
	if (core)
	{
		scene_release(&core->scene);
		img_release(&core->mlx, &core->frame);
		ocl_release(&core->ocl, NULL, 0);
	}
	exit(EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}

int					on_key_pressed(int key, void *core)
{
	if (key == ESC_KEY)
		return (exit_rt(core));
	else if (key == W_KEY || key == A_KEY || key == S_KEY || key == D_KEY ||
			key == SHIFT_KEY || key == SPACE_KEY)
		move_cam(core, key);
	else if (key == ARROW_UP_KEY || key == ARROW_LEFT_KEY ||
			key == ARROW_DOWN_KEY || key == ARROW_RIGHT_KEY ||
			key == PG_UP || key == PG_DOWN)
		rotate_cam(core, key);
	return (0);
}

int					on_window_closing(void *core)
{
	return (exit_rt(core));
}

int					on_tick(void *c)
{
	t_rt			*core;
	t_timer			timer;

	core = (t_rt *)c;
	if (core->should_update)
	{
		timer_start(&timer);
		if (render_frame(core) != CL_SUCCESS)
			ft_putendl_fd("\x1b[93mWARNING\x1b[0m: failed to render frame !!",
					STDERR_FILENO);
		else
			mlx_put_image_to_window(core->mlx.mlx_ptr, core->mlx.win_ptr,
				core->frame->mlx_img, 0, 0);
		core->should_update = 0;
		timer_end(&timer);
		timer_display(&timer, "frame time: ");
	}
	return (0);
}
