/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IInteractive.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 10:16:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/20 15:39:09 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IINTERACTIVE_HPP
# define IINTERACTIVE_HPP

# include "glm/glm.hpp"
# include "Input.hpp"

class IInteractive
{
	public :

		IInteractive(void);
		virtual ~IInteractive(void);

		virtual bool	update_interaction(Input const &input,
							float input_timer) = 0;
		virtual bool	getPosUpdateRequest(void) const = 0;
		virtual void	setPosition(glm::vec3 const &pos) = 0;

	private :

		IInteractive(IInteractive const &src);
		virtual IInteractive		&operator=(IInteractive const &rhs);
};

#endif
