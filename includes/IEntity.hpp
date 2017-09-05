/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEntity.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 11:44:29 by cledant           #+#    #+#             */
/*   Updated: 2017/09/05 16:02:00 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IENTITY_HPP
# define IENTITY_HPP

class IEntity
{
	public :

		IEntity(void);
		virtual ~IEntity(void);

		void		update(float time) = 0;
		void		draw(void) = 0;

	private :

		IEntity(IEntity const &src);
		virtual IEntity		&operator=(IEntity const &rhs);
};

#endif
