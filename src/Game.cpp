/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:56:03 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/25 18:22:32 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

#include "Tab.hpp"
#include "Button.hpp"
#include "Text.hpp"

#include <iostream>
void myFunction(UI &ui) {
    std::cout << "Button clicked!" << std::endl;
}

void lol(UI &ui)
{
    Text &t = dynamic_cast<Text &>(ui);
    std::cout << "got tis: "<< t.get_text() << std::endl;
    t.get_text().clear();
}

Game::Game() : UIWin("tiboon", 800, 600, 60)
{
    add_ui(std::make_unique<Tokel>(10, 10, 100, 50, "Tokel"));
    add_ui(std::make_unique<Tokel>(10, 100, 100, 50, "Tokel2"));
    add_ui(std::make_unique<Button>(10, 200, 100, 50, "button3", myFunction));
    add_ui(std::make_unique<Button>(10, 300, 100, 50, "button3", myFunction));
    add_ui(std::make_unique<Text>(10, 400, 100, 50, lol));
    add_ui(std::make_unique<Tab>(10, 500, 100, 50, std::initializer_list<std::string>{"tab1", "tab2", "tab3"}));
}

Game::~Game()
{
}

void Game::draw() const
{
    ClearBackground(RAYWHITE);
    UIWin::draw();
}

void Game::update()
{
    UIWin::update();
}