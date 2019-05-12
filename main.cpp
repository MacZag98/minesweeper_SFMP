#include <iostream>
#include <C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\include\SFML/Graphics.hpp>
#include "Board.h"


int main()
{
    sf::RenderWindow minesweeper(sf::VideoMode(WIDTH*SIZE_OF_FIELD, HEIGHT*SIZE_OF_FIELD), "Minesweeper",sf::Style::Titlebar); //utworzenie okna
    Board NewGame = Board(&minesweeper); //utworzenie nowej gry
    while (minesweeper.isOpen()) //jesli aplikacja jest otwarta
    {
        sf::Vector2i pos = sf::Mouse::getPosition(minesweeper); //utworzenie zmiennej pozycji kursora w oknie
        int x = pos.x/SIZE_OF_FIELD; //zczytanie wspolrzednej x kursora podzielone przez rozmiar pola
        int y = pos.y/SIZE_OF_FIELD; //zczytanie wspolrzednej y kursora i podzielenie przez rozmiar pola
        sf::Event event; //nowe zdarzenie
        while (minesweeper.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) //jesli okno zostanie zamknete
                minesweeper.close();
            if (event.type == sf::Event::MouseButtonPressed) //jesli zostanie wcisniety przycisk myszy
                if (event.key.code == sf::Mouse::Left) //lewy
                {
                    if(!NewGame.discover_field(x,y)) //odkrywa pole jesli na polu jest mina to
                    {
                        NewGame.discover_all_mines(&minesweeper); //odkrywa wszystkie miny
                        while (minesweeper.isOpen())
                        {
                            sf::Event event; //nowe zdarzenie
                            while (minesweeper.pollEvent(event))
                            {
                                if (event.type == sf::Event::Closed) //jesli okno zostanie zamknete
                                    minesweeper.close();
                                if (event.type == sf::Event::MouseButtonPressed) //jesli zostanie wcisniety przycisk myszy
                                    if (event.key.code == sf::Mouse::Left) //lewy
                                    {
                                        return 0; //konczy dzialanie
                                    }
                            }
                        }
                    }
                }
                else if (event.key.code == sf::Mouse::Right) //prawy
                    NewGame.flag_or_deflag_field(x, y); //flauje lub odflagowuje pole
            if (NewGame.display(&minesweeper)) //wyswietla plansze. jesli wygrana to:
            {
            while (minesweeper.isOpen())
            {
                sf::Event event; //nowe zdarzenie
                while (minesweeper.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed) //jesli okno zostanie zamknete
                        minesweeper.close();
                    if (event.type == sf::Event::MouseButtonPressed) //jesli zostanie wcisniety przycisk myszy
                        if (event.key.code == sf::Mouse::Left) //lewy
                        {
                            return 0; //konczy dzialanie
                        }
                }
            }
            }
        }
    }
}
