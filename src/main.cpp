#include <chrono>
#include <iostream>
#include <thread>

#include <terminal_ctrl.hpp>

bool running {true};
bool menu {true};
bool game {false};

// dessine le menu du jeu
void draw_menu() {
    std::cout << "Snake template\n";
    std::cout << "Appuyez sur Echap pour quitter\n";
    std::cout << "Lancez le jeu avec Entrée\n";
}

// dessine le jeu
void draw_game() {
    std::cout << "en jeu\n";
}

// revient au menu depuis le jeu, et quitte le jeu depuis le menu
void retour(){
    if(menu == true){
        running = false;
    } else if(game == true){
        menu = true;
        game = false;
    }
}

int main() {
    // Active le mode "raw" du terminal sous Linux et MacOS pour lire les touches sans appuyer sur Entrée. Sous Windows, ne fait rien car _getch() lit déjà les touches directement
    // Ce mode raw est automatiquement désactivé à la fin du programme
    terminal_ctrl::RawModeGuard raw_mode {};
    terminal_ctrl::save_default_attributes();

    // Cache le curseur pour éviter les clignotements pendant le redessin.
    terminal_ctrl::set_cursor_visibility(false);

    while (running) {
        // Efface l'écran et replace le curseur en haut à gauche
        terminal_ctrl::clear_screen();
        terminal_ctrl::set_cursor_position(1, 1);

        // fonctions appellées sur le menu
        if(menu == true){
            draw_menu();
        }

        // fonctions appellées sur le jeu
        if(game == true){
            draw_game();
        }

        // Vide toutes les touches en attente sans bloquer la boucle de jeu
        while (std::optional<terminal_ctrl::KeyCode> const key { terminal_ctrl::get_key_non_blocking() }) {

            if (*key == terminal_ctrl::KeyCode::KEY_ESCAPE) {
                retour();
            }

            if (*key == terminal_ctrl::KeyCode::KEY_ENTER) {
                menu = false;
                game = true;
            }
        }

        // Simule une frame avec une pause: ajustez la durée pour changer la vitesse du jeu
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    terminal_ctrl::restore_default_attributes();
    // Restaure le curseur et efface l'écran avant de quitter
    terminal_ctrl::set_cursor_visibility(true);
    terminal_ctrl::clear_screen();
}