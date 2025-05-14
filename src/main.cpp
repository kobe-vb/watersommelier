/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:04:09 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/13 13:18:17 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Game.hpp"

int main(void)
{
    Game().run();
    return 0;
}

        // working.clear();
        // int key = GetCharPressed();
        // while (key > 0) {
        //     working += (char)key;
        //     key = GetCharPressed();
        // }
        // if (working.length() > 2)
        //     barcodeInput = working;