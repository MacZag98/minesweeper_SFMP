#include "Board.h"
#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>


Board::Board(sf::RenderWindow *app)
{
    discover_all_mines(app); //wyswietla pusta plansze
    sf::Vector2i pos = sf::Mouse::getPosition(*app); //utworzenie zmiennej pozycji kursora w oknie
    int x_start = pos.x/SIZE_OF_FIELD, //zczytanie wspolrzednej x kursora podzielone przez rozmiar pola
        y_start = pos.y/SIZE_OF_FIELD,  //zczytanie wspolrzednej y kursora i podzielenie przez rozmiar pola
        number_of_mines = NUMBER_OF_MINES, //liczba min
        x, y; // wspolrzedne zmienne
        while (app->isOpen())
        {
            pos = sf::Mouse::getPosition(*app); //zczytanie zmiennej pozycji kursora w oknie
            x_start = pos.x/SIZE_OF_FIELD; //zczytanie wspolrzednej x kursora podzielone przez rozmiar pola
            y_start = pos.y/SIZE_OF_FIELD; //zczytanie wspolrzednej y kursora i podzielenie przez rozmiar pola
            sf::Event event; //nowe zdarzenie
            if (app->pollEvent(event))
                if (event.type == sf::Event::MouseButtonPressed) //jesli zostaje wcisniety przycisk myszy
                    if (event.key.code == sf::Mouse::Left)
                    {
                        game_field[x_start][y_start].if_discovered=1; //odkrywa poczatkowe pole
                        break;
                    }
        }
    srand( time( NULL ) ); //ustawia ziarno
    while (number_of_mines!=0) //wybiera pola i umieszcza na nich miny
    {
        x=rand()%WIDTH; //wybiera pierwsza wspolrzedna
        y=rand()%HEIGHT; //wybiera druga wspolrzedna
        if (game_field[x][y].if_mined==0 && !game_field[x][y].if_discovered) //jesli na polu nie ma miny i jest nieodkryte
        {
            game_field[x][y].if_discovered=0;
            game_field[x][y].if_flaged=0;
            game_field[x][y].if_mined=1; //dodaje mine
            number_of_mines--;
        }
    }
    discover_adjacent_0(x_start, y_start); //odkrywa puste pola dookola startowego
}

Board::~Board()
{
    delete game_field;
}

bool Board::discover_field(int x, int y) //odkrywa wybrane pole zwraca falsz jesli zostanie odkryte pole z mina
{
    if (!game_field[x][y].if_flaged) //sprawdza czy pole nie jest oflagowane, jesli jest to nie mozna odkryc
        {
        if (game_lost(x,y)) //sprawdza czy jest mina
            return false;
        game_field[x][y].if_discovered=1;
        discover_adjacent_0 (x, y);
        }
    return true;
}

void Board::discover_adjacent_0 (int x, int y) //odkrywa sasiadujace pola z 0
{
    if (count_adjacent_mines(x, y)==0)
    {
        for (int i1=-1; i1<2; i1++)
            for (int j1=-1; j1<2; j1++)
                if (-1<x+i1 && x+i1<WIDTH && -1<y+j1 && y+j1<HEIGHT && //pole musi byc sasiadujace
                    !game_field[x+i1][y+j1].if_discovered && //pole musi byc nieodkryte
                    (count_adjacent_mines(x+i1,y+j1)==0 || //sasiadujace pole musi zawierac 0
                     (!game_field[x+i1][y+j1].if_mined && count_adjacent_mines(x+i1,y+j1)>0))) //lub musi byc niezaminowanym polem na ktorym powinna pojawic sie liczba
                        discover_field(x+i1,y+j1); //w takim wypadku odkrywa pole
    }
}

void Board::discover_all_mines(sf::RenderWindow *app) //odkrywa pozycje min na planszy po przegranej grze
{
    sf::Texture t; //tworzy zmienna tekstury
    t.loadFromFile(TEXTURE); //zaladowuje teksture
    sf::Sprite s(t); //tworzy wycinek z tekstury t
    for (int i=0; i<HEIGHT; i++)
    {
        for (int j=0; j<WIDTH; j++)
            {
                if (!game_field[j][i].if_discovered && !game_field[j][i].if_flaged)
                    if (game_field[j][i].if_mined)  //rysuje pole z mina
                        {
                            s.setTextureRect(sf::IntRect(9*SIZE_OF_FIELD,0,SIZE_OF_FIELD,SIZE_OF_FIELD));
                            s.setPosition(j*SIZE_OF_FIELD, i*SIZE_OF_FIELD);
                            app->draw(s);
                        }
                    else //rysuje pole nieodkryte
                        {
                            s.setTextureRect(sf::IntRect(10*SIZE_OF_FIELD,0,SIZE_OF_FIELD,SIZE_OF_FIELD));
                            s.setPosition(j*SIZE_OF_FIELD, i*SIZE_OF_FIELD);
                            app->draw(s);
                        }
                if (!game_field[j][i].if_discovered && game_field[j][i].if_flaged)
                    if (game_field[j][i].if_mined) //rysuje pole z mina
                        {
                            s.setTextureRect(sf::IntRect(9*SIZE_OF_FIELD,0,SIZE_OF_FIELD,SIZE_OF_FIELD));
                            s.setPosition(j*SIZE_OF_FIELD, i*SIZE_OF_FIELD);
                            app->draw(s);
                        }
                    else //rysuje pole z flaga
                        {
                            s.setTextureRect(sf::IntRect(11*SIZE_OF_FIELD,0,SIZE_OF_FIELD,SIZE_OF_FIELD));
                            s.setPosition(j*SIZE_OF_FIELD, i*SIZE_OF_FIELD);
                            app->draw(s);
                        }
                if (game_field[j][i].if_discovered)
                    if (count_adjacent_mines(j,i)==0) //rysuje puste odkryte pole
                    {
                        s.setTextureRect(sf::IntRect(0,0,SIZE_OF_FIELD,SIZE_OF_FIELD));
                        s.setPosition(j*SIZE_OF_FIELD, i*SIZE_OF_FIELD);
                        app->draw(s);
                    }
                    else //rysuje pole z liczba
                    {
                        s.setTextureRect(sf::IntRect(count_adjacent_mines(j,i)*SIZE_OF_FIELD,0,SIZE_OF_FIELD,SIZE_OF_FIELD));
                        s.setPosition(j*SIZE_OF_FIELD, i*SIZE_OF_FIELD);
                        app->draw(s);
                    }
            }
    }
    app->display();
}

void Board::flag_or_deflag_field (int x, int y) //flaguje lub odflagowuje pole
{
    if (!game_field[x][y].if_flaged)
        game_field[x][y].if_flaged=1;
    else
        game_field[x][y].if_flaged=0;
}

int Board::count_adjacent_mines(int x, int y) //funkcja zlicza ile pol z minami sasiaduje z wybranym
{
    int counter=0;
    for (int i1=-1; i1<2; i1++)
        for (int j1=-1; j1<2; j1++)
            if (-1<x+i1 && x+i1<WIDTH && -1<y+j1 && y+j1<HEIGHT && //pole musi byc sasiadujace
                !game_field[x+i1][y+j1].if_discovered && //pole musi byc nieodkryte
                game_field[x+i1][y+j1].if_mined) //pole musi byc zaminowane
                    counter++; //zwieksza licznik min
    return counter; //zwraca licznik min
}

bool Board::display(sf::RenderWindow *app) //wyswietla plansze i sprawdzaczy gra zostala wygrana
{
    int number_of_mines = NUMBER_OF_MINES;
    sf::Texture t; //tworzy zmienna tekstury
    t.loadFromFile(TEXTURE); //zaladowuje teksture
    sf::Sprite s(t); //tworzy wycinek z tekstury t
    int do_I_win=0;
    for (int i=0; i<HEIGHT; ++i)
    {
        for (int j=0; j<WIDTH; ++j)
            {
                if (!game_field[j][i].if_discovered && !game_field[j][i].if_flaged) //rysuje pole nieodkryte
                {
                    s.setTextureRect(sf::IntRect(10*SIZE_OF_FIELD,0,SIZE_OF_FIELD,SIZE_OF_FIELD));
                    s.setPosition(j*SIZE_OF_FIELD, i*SIZE_OF_FIELD);
                    app->draw(s);
                    ++do_I_win;
                }
                if (!game_field[j][i].if_discovered && game_field[j][i].if_flaged) //rysuje pole z flaga
                {
                    s.setTextureRect(sf::IntRect(11*SIZE_OF_FIELD,0,SIZE_OF_FIELD,SIZE_OF_FIELD));
                    s.setPosition(j*SIZE_OF_FIELD, i*SIZE_OF_FIELD);
                    app->draw(s);
                    ++do_I_win;
                }
                if (game_field[j][i].if_discovered)
                    if (count_adjacent_mines(j,i)==0) //rysuje puste odkryte pole
                    {
                        s.setTextureRect(sf::IntRect(0,0,SIZE_OF_FIELD,SIZE_OF_FIELD));
                        s.setPosition(j*SIZE_OF_FIELD, i*SIZE_OF_FIELD);
                        app->draw(s);
                    }
                    else //rysuje pole z liczba
                    {
                        s.setTextureRect(sf::IntRect(count_adjacent_mines(j,i)*SIZE_OF_FIELD,0,SIZE_OF_FIELD,SIZE_OF_FIELD));
                        s.setPosition(j*SIZE_OF_FIELD, i*SIZE_OF_FIELD);
                        app->draw(s);
                    }
            }
    }
    if (do_I_win==number_of_mines)
    {
        app->display(); //wyswietla stan w oknie
        return true;
    }
    else
    {
        app->display(); //wyswietla stan w oknie
        return false;
    }
}
