/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprite_column.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 04:30:37 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/03/15 19:01:49 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../cube.h"

int determine_sprite_texture(t_ent *sprite, int screen_column, int total_rays)
{
    int texture_column;

    texture_column = (screen_column * sprite->frame->size.x) / total_rays;
    if (texture_column < 0)
        texture_column = 0;
    else if (texture_column >= sprite->frame->size.x)
        texture_column = sprite->frame->size.x - 1;

    return texture_column;
}

int draw_sprite_column(t_md *md, t_image *img, t_vec2 win_pos, t_vec3f img_coords)
{
    t_vec2 y;
    int pixel;
    int img_offset;
    int img_y;
    int vertical_end;
    float step;

    vertical_end = md->win_size.y;
    y.x = (md->win_size.y / 2 - img_coords.y / 2) - 1;
    step = img->size.y / img_coords.y;
    y.y = (md->win_size.y / 2 + img_coords.y / 2);

    while (++y.x < y.y)
    {
        img_y = (y.x - (md->win_size.y / 2 - img_coords.y / 2)) * step;
        if (img_y < 0 || img_y >= img->size.y)
            continue;

        img_offset = ((int)img_y * (img->size_line / 4)) + (int)img_coords.x;
        pixel = *(img->src_data + img_offset);

        if ((pixel >> 24) != 0x00) // Ignore transparent pixels
            continue;

        vertical_end = y.x + win_pos.y - md->plr.pos.z;

        // Draw the pixel at the correct position
        draw_pixel(md->screen, get_v2(win_pos.x, y.x + win_pos.y - md->plr.pos.z), pixel, -1);
    }

    return vertical_end;
}

void draw_sprite_pxl(t_md *md, t_ray *ray, t_ent *sprite, t_vec3f win_pos)
{
    t_vec2 draw_start;
    t_image *img;
    float vrt_offset;
    int vertical_end;
    int texture_column;

    vrt_offset = compute_perspective_change(md, NULL, ray->distance);
    img = sprite->frame;  // Get sprite frame
    texture_column = determine_sprite_texture(sprite, ray->index, md->win_size.x);
	(void)texture_column;
    draw_start = get_v2(ray->index, vrt_offset);
    vertical_end = draw_sprite_column(md, img, draw_start, win_pos);
    if (vertical_end < md->hud.floor_start)
        md->hud.floor_start = vertical_end;
    sprite->row_draw_index++;
}

void draw_sprite(t_md *md, float dist, t_ent *sprite, t_ray *ray, int total_rays)
{
    t_vec3f win_pos;
    float fisheye_corrector;

    ray->distance = maxf(0.01, dist);

    fisheye_corrector = (md->win_size.y * sprite->size.y) /
                        (dist * (fabs(ray->angle - md->plr.angle) + 0.0001f));
	fisheye_corrector = (md->win_size.y * sprite->size.y) / 
	(dist * fabs(cos(ray->angle - md->plr.angle)));
    win_pos.x = determine_sprite_texture(sprite, sprite->row_draw_index, total_rays);
    win_pos.y = fisheye_corrector;
    draw_sprite_pxl(md, ray, sprite, win_pos);
}
