/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llopez <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 22:12:07 by llopez            #+#    #+#             */
/*   Updated: 2019/02/03 23:49:33 by llopez           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

t_tube		*hubble(t_tube *room, t_tube *from, t_infos *infos, int nb)
{
	int		ret;
	int		ret_minus;
	t_tube	*minus_room;
	t_paths *tmp;

	tmp = room->links;
	infos->ret_minus = 0;
	minus_room = NULL;
	ret_minus = 0;
	while (tmp)
	{
		if (tmp->room != from && (ret = visitable(tmp, room, infos, nb)))
		{
			infos->radius = (ret == nb + 1) ? ret : infos->radius;
			if (!ret_minus || ret < ret_minus)
			{
				ret_minus = ret;
				minus_room = tmp->room;
			}
		}
		tmp = tmp->next;
	}
	infos->ret_minus = ret_minus;
	return (minus_room);
}

int			voyager(t_tube *room, t_tube *from, t_infos *infos, int nb)
{
	t_tube	*minus_room;

	minus_room = NULL;
	if (room->passages > 15000)
		return (0);
	room->passages++;
	if (check_start(room->links, infos))
		return (nb);
	room->vu = 1;
	if (!(minus_room = hubble(room, from, infos, nb)))
		return (0);
	minus_room->pass = nb + 1;
	return (infos->ret_minus);
}

int			set_position(t_tube *room, t_tube *from, t_infos *infos, int nb)
{
	t_paths	*links;

	links = room->links;
	while (links)
	{
		if (links->room == infos->start)
		{
			room->steps = (!room->steps || room->steps > nb) ? nb : room->steps;
			return (1);
		}
		links = links->next;
	}
	links = room->links;
	while (links && (room->vu = 2))
	{
		if (links->room != from && links->room->pass == nb + 1\
			&& links->room->vu != 2\
			&& set_position(links->room, room, infos, nb + 1))
		{
			room->steps = (!room->steps || room->steps > nb) ? nb : room->steps;
			return (1);
		}
		links = links->next;
	}
	return (0);
}

void		reset_view(t_tube *room)
{
	while (room->prev)
		room = room->prev;
	while (room)
	{
		room->vu = 0;
		room = room->next;
	}
}

int			find_path(t_infos *infos)
{
	t_paths	*links;
	int		total;
	int		ret;

	total = 0;
	links = infos->end->links;
	while (links)
	{
		infos->radius = 0;
		if (links->room == infos->start)
			total++;
		if ((ret = voyager(links->room, infos->end, infos, 1)))
			set_position(links->room, infos->end, infos, 1);
		total = (ret) ? total + 1 : total;
		reset_view(infos->start);
		links = links->next;
	}
	return (total);
}
