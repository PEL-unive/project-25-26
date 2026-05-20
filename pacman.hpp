#include <cassert>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <string>
#include <thread>
#include <chrono>
#include<sstream>

constexpr uint32_t PANIC_RESET = 80;

enum class cell_type {
    empty,
    wall,
    pellet,
    power_pellet,
    pacman,
    ghost1,
    ghost2,
    ghost3,
    ghost4
};

struct pacman_exception {
    pacman_exception(std::string const& msg) : m_msg(msg) {}
    std::string const& what() const { return m_msg; }

private:
    std::string m_msg;
};

struct game_state {
    game_state();
    game_state(uint32_t s);
    game_state(game_state const& rhs);
    game_state(game_state&& rhs);
    game_state& operator=(game_state const& rhs);
    game_state& operator=(game_state&& rhs);
    ~game_state();

    uint32_t get_size() const;
    uint32_t get_score() const;
    uint32_t get_lives() const;
    uint32_t get_pellets_left() const;
    uint32_t get_panic_countdown() const;
    bool win() const;

    void set_size(uint32_t s);
    void set_score(uint32_t score);
    void set_lives(uint32_t lives);
    void set_pellets_left(uint32_t pellets_left);
    void set_panic_countdown(uint32_t panic_countdown);

    cell_type& operator()(uint32_t i, uint32_t j);
    cell_type operator()(uint32_t i, uint32_t j) const;

    bool operator==(game_state const& rhs) const;
    bool operator!=(game_state const& rhs) const;

    void print_ascii_art(std::ostream& os) const;

private:
    uint32_t m_size;
    uint32_t m_score;
    uint32_t m_lives;
    uint32_t m_pellets_left;
    uint32_t m_panic_countdown;
    cell_type** m_grid;
};

std::ostream& operator<<(std::ostream& os, game_state const& gs);
std::istream& operator>>(std::istream& is, game_state& gs);

class pacman {
    struct node {
        game_state gs;
        node* next;
    };

public:
    pacman();
    pacman(uint32_t size);
    pacman(pacman const& rhs);
    pacman(pacman&& rhs);
    pacman& operator=(pacman const& rhs);
    pacman& operator=(pacman&& rhs);
    ~pacman();

    uint32_t size() const;
    uint32_t length() const;
    bool empty() const;

    game_state const& front() const;
    game_state const& back() const;

    bool operator==(pacman const& rhs) const;
    bool operator!=(pacman const& rhs) const;

    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = game_state;
        using reference = game_state&;
        using pointer = game_state*;

        iterator(node* ptr);
        reference operator*();
        pointer operator->();
        iterator& operator++();
        iterator operator++(int);
        bool operator==(iterator const& rhs) const;
        bool operator!=(iterator const& rhs) const;

    private:
        node* m_ptr;
    };

    struct const_iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = game_state const;
        using reference = game_state const&;
        using pointer = game_state const*;

        const_iterator(node const* ptr);
        reference operator*() const;
        pointer operator->() const;
        const_iterator& operator++();
        const_iterator operator++(int);
        bool operator==(const_iterator const& rhs) const;
        bool operator!=(const_iterator const& rhs) const;

    private:
        node const* m_ptr;
    };

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    void push_back(game_state const& gs);
    void move(cell_type who, int delta_i, int delta_j);

    void move(cell_type who); //facultative and not evaluated! with this you can implement a strategy (AI) that automatically plays the game

    void print_ascii_art(std::ostream& os) const;

private:
    uint32_t m_size;
    uint32_t m_length;
    node* m_head;
    node* m_tail;
};

std::ostream& operator<<(std::ostream& os, pacman const& g);
std::istream& operator>>(std::istream& is, pacman& g);
