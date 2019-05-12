#define SIZE_OF_FIELD 32
#define TEXTURE "texture32x32.jpg"
#define HEIGHT 16
#define WIDTH 31
#define NUMBER_OF_MINES 99
#include <SFML/Graphics.hpp>

class Board
{
private:
    struct Field //struktura pole
    {
        bool if_mined=0; //czy zaminowane
        bool if_discovered=0; //czy odkryte
        bool if_flaged=0; //czy oflagowane
    };
    Field game_field[WIDTH][HEIGHT];
public:
    Board(); //pusty konstruktor
    Board(sf::RenderWindow *app); //konstruktor z podaniem liczby min
    ~Board(); //destruktor
    bool display (sf::RenderWindow *window); //wyswietla plansze i sprawdzaczy gra zostala wygrana
    bool discover_field(int x, int y); //odkrywa wybrane pole zwraca falsz jesli zostanie odkryte pole z mina
    void discover_adjacent_0 (int x, int y); //odkrywa sasiadujace pola z 0
    void flag_or_deflag_field (int x, int y); //flaguje lub odflagowuje pole
    int count_adjacent_mines(int x, int y); //funkcja zlicza ile pol z minami sasiaduje z wybranym
    bool game_lost(int x, int y) {if (game_field[x][y].if_mined) return true; return false;} //sprawdza czy pole wybrane do odkrycia jest zaminowane
    void discover_all_mines (sf::RenderWindow *app); //odkrywa pozycje min na planszy po przegranej grze
};


