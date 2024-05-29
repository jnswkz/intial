/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };

class MapElement {
protected:
    ElementType type;
public:
    friend class TestStudyPink;
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement {
public:
    friend class TestStudyPink;
    Path();
};

class Wall : public MapElement {
public:
    friend class TestStudyPink;
    Wall();
};

class FakeWall : public MapElement {
private:
    int req_exp;
public:
    friend class TestStudyPink;
    FakeWall(int in_req_exp);
    int getReqExp() const;
};

class Map {
private:
    int num_rows, num_cols;
    MapElement *** map;
public:
    friend class TestStudyPink;
    Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);// : num_rows(num_rows), num_cols(num_cols);
    ~Map();
    bool isValid( const Position & pos , MovingObject * mv_obj ) const ;
    MapElement *** getMap() const { return map; };
    int getNumRows() const { return num_rows; };
    int getNumCols() const { return num_cols; };

    void printMap() ;
};

class Position {
private:
    int r, c;
public:
    friend class TestStudyPink;

    static const Position npos;

    Position(int r=0, int c=0);

    Position(const string & str_pos);

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);

    string str() const;

    bool isEqual(int in_r, int in_c) const;
    bool isEqual(const Position & pos) const;
};

class MovingObject {
protected:
    int index;
    Position pos;
    Map * map;
    string name;

public:
    friend class TestStudyPink;

    MovingObject(int index, const Position pos, Map * map, const string & name="");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;
    void setPosition(const Position & pos);
    void setMap(Map * map);
    int getIndex() const;
    string getName() const;
    void setName(const string & name);
    void setIndex(int index);
};

class Character : public MovingObject {
protected:
    int hp;
    int exp;
    string moving_rule;
public:
    friend class TestStudyPink;
    Character(int index, const Position & pos, Map * map, int hp, int exp, const string & name, string moving_rule);
    virtual ~Character();
    int getHp() const;
    int getExp() const;
    void setHp(int hp);
    void setExp(int exp);
};

class Sherlock : public Character {
private:
    string cur_moving_rule;

public:
    friend class TestStudyPink;
    Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    // getNextPosition
    // move
    // str
    // ...
    Position getNextPosition() override;
    void move() override;
    string str() const override;

    void sherlock_action(ArrayMovingObject * arr_mv_objs, SherlockBag * sherlock_bag, WatsonBag * watson_bag);

};

class Watson : public Character{
private:
    // TODO
    string cur_moving_rule;


public:
    friend class TestStudyPink;
    Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    // getNextPosition
    // move
    // str
    // ...
    Position getNextPosition() override;
    void move() override;
    string str() const override;

    void watson_action(ArrayMovingObject * arr_mv_objs, SherlockBag * sherlock_bag, WatsonBag * watson_bag);

};

class Criminal : public MovingObject {
private:
    // TODO
    Sherlock * sherlock;
    Watson * watson;
    int nsteps = 0;
    Position previous_pos;

public:
    friend class TestStudyPink;
    Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson);
    // getNextPosition
    // move
    // str
    // ...
    Position getNextPosition() override;
    Position getPreviousPosition() const;
    void move() override;
    string str() const override;
    void createRobot(ArrayMovingObject * arr_mv_objs);
};

class ArrayMovingObject {
private:
    // TODO
    MovingObject ** arr_mv_objs;
    int count;
    int capacity;
public:
    friend class TestStudyPink;
    ArrayMovingObject(int capacity);

    ~ArrayMovingObject() ;
    bool isFull() const;
    bool add(MovingObject * mv_obj);
    MovingObject * get(int index) const;
    int size() const; // return current number of elements in the array
    string str() const;
};

class Configuration {
    friend class StudyPinkProgram;

private:
    // TODO
    int map_num_rows, map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position * arr_walls;
    int num_fake_walls;
    Position * arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;

public:
    friend class TestStudyPink;
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
    int getNumRows() const ;
    int getNumCols() const ;
    int getMaxNumMovingObjects() const ;
    int getNumWalls() const ;
    Position *getArrWalls() const ;
    int getNumFakeWalls() const ;
    Position *getArrFakeWalls() const ;
    string getSherlockMovingRule() const ;
    Position getSherlockInitPos() const ;
    int getSherlockInitHp() const ;
    int getSherlockInitExp() const ;
    string getWatsonMovingRule() const ;
    Position getWatsonInitPos() const ;
    int getWatsonInitHp() const ;
    int getWatsonInitExp() const ;
    Position getCriminalInitPos() const ;
    int getNumSteps() const ;
};

// Robot, BaseItem, BaseBag,...

// Robot
class Robot : public MovingObject{
private:
    RobotType robot_type;
    BaseItem * item;
public:
    friend class TestStudyPink;
    Robot(int index, const Position& init_pos, Map * map, Criminal * criminal);
    ~Robot();
    Criminal * criminal;
    RobotType getRobotType() const;
    BaseItem * getItem() const;
    void setItem(BaseItem * item);
    void setRobot_type(RobotType robot_type);
    
    int getDistance(Sherlock * sherlock) const;
    int getDistance(Watson * watson) const;

    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
};

class RobotC : public Robot {
public:
    friend class TestStudyPink;
    RobotC(int index, const Position& init_pos, Map * map, Criminal * criminal);
    Position getNextPosition() override ;
    void move() override;
    string str() const override;

};

class RobotS : public Robot {
public:

    friend class TestStudyPink;
    
    Sherlock * sherlock;
    RobotS(int index, const Position& init_pos, Map * map, Criminal * criminal, Sherlock * sherlock);
    Position getNextPosition() override;
    void move() override;
    string str() const override;

    int getDistance() const {
        return Robot::getDistance(sherlock);
    };

};

class RobotW : public Robot {
public:
    friend class TestStudyPink;
    Watson * watson;
    RobotW(int index, const Position& init_pos, Map * map, Criminal * criminal, Watson * watson);
    Position getNextPosition() override;
    void move() override;
    string str() const override;

    int getDistance() const {
        return Robot::getDistance(watson);
    }
};

class RobotSW : public Robot {
public:
    friend class TestStudyPink;
    Sherlock * sherlock;
    Watson * watson;
    RobotSW(int index, const Position& init_pos, Map * map, Criminal * criminal, Sherlock * sherlock, Watson * watson);
    Position getNextPosition() override;
    void move() override;
    string str() const override;

    int getDistance() const;
};

// Item
class BaseItem {

public :
    friend class TestStudyPink;
    
    string item_name;

    BaseItem(string item_name);

    virtual bool canUse(Character * character, Robot * robot = NULL) = 0;
    virtual void use(Character * character, Robot * robot = NULL) = 0;

    string getItemName() const {
        return item_name;
    }
};

class MagicBook : public BaseItem {
public:
    friend class TestStudyPink;
    MagicBook();
    bool canUse(Character * character, Robot * robot = NULL) override;
    void use(Character * character, Robot * robot = NULL) override;

};

class EnergyDrink : public BaseItem {
public:
    friend class TestStudyPink;
    EnergyDrink();
    bool canUse(Character * character, Robot * robot = NULL) override;
    void use(Character * character, Robot * robot = NULL) override;
};


class FirstAid : public BaseItem {

public:
    friend class TestStudyPink;
    FirstAid();
    bool canUse(Character * character, Robot * robot = NULL) override;
    void use(Character * character, Robot * robot = NULL) override;
};

class ExcemptionCard : public BaseItem {

public:
    friend class TestStudyPink;
    ExcemptionCard();
    bool canUse(Character * character, Robot * robot = NULL) override;
    void use(Character * character, Robot * robot = NULL) override;
};

class PassingCard : public BaseItem {

public:
    friend class TestStudyPink;

    string challenge;
    PassingCard(const string & challenge);
    bool canUse(Character * character, Robot * robot = NULL) override;
    void use(Character * character, Robot * robot = NULL) override;
};

//BaseBag

typedef struct node{
    BaseItem * item;
    node * next;

    node(BaseItem * item , node *next = NULL){
        this -> item = item;
        this -> next = next;
    }
}node;

class BaseBag{
protected :
    Character * obj;
    int count = 0;
    int capacity ;
    node * head = NULL;
public:
    friend class TestStudyPink;

    BaseBag(Character * obj, int capacity);
    virtual ~BaseBag();
    virtual bool insert(BaseItem * item);
    virtual BaseItem * get();
    virtual BaseItem * get(ItemType itemType);
    virtual string str()const ;
};

class SherlockBag : public BaseBag {
public:
    friend class TestStudyPink;

    SherlockBag(Sherlock * sherlock);
};

class WatsonBag : public BaseBag {
public:
    friend class TestStudyPink;

    WatsonBag(Watson * watson);
};



class StudyPinkProgram {
private:
    // Sample attributes
    Configuration * config;
    
    Sherlock * sherlock;
    Watson * watson;
    Criminal * criminal;
    
    Map * map;
    ArrayMovingObject * arr_mv_objs;

    SherlockBag * sherlock_bag;
    WatsonBag * watson_bag;
    


public:

    friend class TestStudyPink;
    StudyPinkProgram(const string & config_file_path);

    bool isStop() const;

    void printResult() const {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Watson caught the criminal" << endl;
        }
        else {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    void run(bool verbose) ;

    Map * getMap() const;

    ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
