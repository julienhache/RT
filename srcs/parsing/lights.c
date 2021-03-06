/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yguaye <yguaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/10 18:38:46 by yguaye            #+#    #+#             */
/*   Updated: 2018/08/23 08:24:34 by jloro            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <libft_base/io.h>
#include "rt.h"

static int			parallel_light(t_light *light, const t_json_value *data)
{
	cl_float3			angle;

	if (!data)
	{
		light->is_parallel = 0;
		return (1);
	}
	else if (!angle_from_json(data, &angle))
		return (1);
	vec3cl_fill(&light->facing, 0, 1, 0);
	rotate_x(&light->facing, angle.x);
	rotate_y(&light->facing, angle.y);
	rotate_z(&light->facing, angle.z);
	light->is_parallel = 1;
	return (1);
}

static int			make_light(t_light *light, const t_json_object *data)
{
	t_json_value	*tmp;

	if (!vec3f_from_json(json_obj_get(data, "pos"), &light->pos))
		return (0);
	if ((tmp = json_obj_get(data, "color")))
	{
		if (!color_from_json(tmp, &light->color))
			return (0);
	}
	else
		vec3cl_fill(&light->color, 1, 1, 1);
	if ((tmp = json_obj_get(data, "power")))
	{
		if (!float_from_json(tmp, &light->power) || light->power < 0
				|| light->power > 5)
			return (0);
	}
	else
		light->power = 1;
	return (parallel_light(light, json_obj_get(data, "parallel_angle")));
}

int					scene_lights(t_scene *scene, const t_json_array *data)
{
	size_t			i;
	t_json_value	*tmp;

	scene->lights_num = data->values_num;
	if (!(scene->lights = malloc(sizeof(t_light) * scene->lights_num)))
		return (0);
	i = 0;
	while (i < scene->lights_num)
	{
		if (!(tmp = json_arr_get(data, i)) || tmp->obj.type != JSON_OBJECT
				|| !make_light(&scene->lights[i], &tmp->obj))
		{
			ft_putstr_fd("Invalid format for light #", STDERR_FILENO);
			ft_putnbr_fd((int)i + 1, STDERR_FILENO);
			ft_putchar_fd('\n', STDERR_FILENO);
			return (0);
		}
		++i;
	}
	return (1);
}
