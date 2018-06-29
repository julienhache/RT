/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ocl_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhache <jhache@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/27 22:08:53 by jhache            #+#    #+#             */
/*   Updated: 2018/06/29 18:45:20 by jhache           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft_base/memory.h>
#include "rt.h"
#include "image.h"
#include "ocl_data.h"

static cl_int		compute_le_frame(t_rt *core, t_kargs *tmp)
{
	unsigned int	i;
	cl_int			ret;
	size_t			glob_dim[3];
	size_t			offset[3];

	glob_dim[1] = core->mem_info.wg_dim[1];
	glob_dim[2] = 1;
	ft_bzero(offset, sizeof(size_t) * 3);
	i = 0;
	while (i < core->mem_info.wg_nb[1] * core->mem_info.wg_nb[0])
	{
		if (core->mem_info.wg_nb[0] - (i % core->mem_info.wg_nb[0])
				< core->mem_info.compute_units)
			glob_dim[0] = core->mem_info.wg_dim[0];
		else
			glob_dim[0] = core->mem_info.wg_dim[0] * core->mem_info.compute_units;
		offset[0] = core->mem_info.wg_dim[0] * (size_t)(i % core->mem_info.wg_nb[0]);
		offset[1] = glob_dim[1] * (size_t)(i / core->mem_info.wg_nb[0]);
		ret = clEnqueueNDRangeKernel(core->ocl.queue, core->ocl.kernel, 2,
				offset, glob_dim, core->mem_info.wg_dim, 0, NULL, NULL);
		if (ret != CL_SUCCESS && !release_kernel_arg(tmp))
			return (ret);
		i += glob_dim[0] / core->mem_info.wg_dim[0];
	}
	return (CL_SUCCESS);
}

cl_int				render_frame(t_rt *core)
{
	t_kargs			*tmp;
	size_t			glob_dim[3];
	cl_int			ret;
	size_t			offset[3];

	if ((tmp = ocl_set_kernel_arg(core, &ret)) == NULL)
		return (ret);
	update_frame_size(core, &core->mem_info);
	if ((ret = compute_le_frame(core, tmp)) != CL_SUCCESS)
		return (ret);
	ft_bzero(offset, sizeof(size_t) * 3);
	SDL_LockSurface(core->frame);
	glob_dim[0] = core->sdl.frame_width;
	glob_dim[1] = core->sdl.frame_height;
	glob_dim[2] = 1;
	ret = clEnqueueReadImage(core->ocl.queue, core->ocl.ocl_img, CL_TRUE,
			offset, glob_dim, 0, 0, core->frame->pixels, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
		return (ret);
	return (print_frame(core) + (int)release_kernel_arg(tmp) * 0);
}
