/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/06 17:41:50 by yguaye            #+#    #+#             */
/*   Updated: 2018/05/13 14:20:41 by yguaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/io.h>
#include <unistd.h>
#include "rtv1.h"

int					main(int argc, char **argv)
{
	t_img			*img;
	t_scene			scene;

	if (argc != 2)
	{
		ft_putendl_fd("Wrong number of arguments", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (!(img = img_make(900, 900)) || !scene_parse(&scene, argv[1]))
	{
		img_release(&img);
		return (EXIT_FAILURE);
	}
	render_frame(&scene, img);
	objs_release(scene.objs, scene.objs_num);
	if (USE_MLX)
		img_mlx_output(img);
	else
		img_ppm_output(img);
	return (EXIT_SUCCESS);
}
