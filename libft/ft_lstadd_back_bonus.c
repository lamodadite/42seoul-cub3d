/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jongmlee <jongmlee@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 16:28:03 by jongmlee          #+#    #+#             */
/*   Updated: 2023/10/16 17:27:23 by jongmlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*cur;
	
	if (lst == NULL || new == NULL)
		return ;
	if (*lst != NULL)
	{
		cur = ft_lstlast(*lst);
		cur->next = new;
	}
	else
		*lst = new;
}
