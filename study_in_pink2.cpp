#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

//support function
Position* stringtoarray(string str, int num) {
    Position* result = (Position*)malloc(sizeof(Position) * num);
    int i = 0;
    int j = 0;
    while (i < str.length()) {
        if (str[i] == '(') {
            int r = 0;
            int c = 0;
            i++;
            while (str[i] != ',') {
                r = r * 10 + (str[i] - '0');
                i++;
            }
            i++;
            while (str[i] != ')') {
                c = c * 10 + (str[i] - '0');
                i++;
            }
            result[j] = Position(r, c);
            j++;
        }
        i++;
    }
    return result;
}

int roundUp(float n){
    return (int)(n-0.000001 + 1);
}

//class Map ELement
MapElement::MapElement(ElementType in_type) {
    this->type = in_type;
}

MapElement::~MapElement() {
    // destructor
}

ElementType MapElement::getType() const {
    return this->type;
}

//class Path
Path::Path() : MapElement(PATH) {
    // constructor
}

//class Wall

Wall::Wall() : MapElement(WALL) {
    // constructor
}

//class FakeWall

FakeWall::FakeWall(int in_req_exp) : MapElement(FAKE_WALL) {
    this->req_exp = in_req_exp;
}

int FakeWall::getReqExp() const {
    return this->req_exp;
}

//class Map
Map::Map(int num_rows, int num_cols, int num_walls, Position* array_walls, int num_fake_walls, Position* array_fake_walls) {
    this->num_rows = num_rows;
    this->num_cols = num_cols;
    MapElement *** tempmap = new MapElement **[num_rows];
    for (int i = 0; i < num_rows; i++) {
        tempmap[i] = new MapElement *[num_cols];
        for (int j = 0; j < num_cols; j++) {
            tempmap[i][j] = new Path();
        }
    }

    for (int i = 0; i < num_walls; i++) {
        int r = array_walls[i].getRow() ;
        int c = array_walls[i].getCol() ;
        // cout << r << " " << c << endl;
        if (r < 0 || r >= num_rows || c < 0 || c >= num_cols) {
            continue;
        }
        delete tempmap[r][c];
        tempmap[r][c] = new Wall();
    }

    for (int i = 0; i < num_fake_walls; i++) {
        int r = array_fake_walls[i].getRow() ;
        int c = array_fake_walls[i].getCol() ;
        int req_exp = (r * 257 + c * 139 + 89) % 900 + 1;
        if (r < 0 || r >= num_rows || c < 0 || c >= num_cols) {
            continue;
        }
        delete tempmap[r][c];
        tempmap[r][c] = new FakeWall(req_exp);
    }

    this->map = tempmap;

}

Map::~Map(){
    //destructer
    this -> num_rows = 0;
    this -> num_cols = 0;
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            delete this->map[i][j];
        }
        delete[] this->map[i];
    }
    delete[] map;
}

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{
    //the position is valid if it is in the map and mv_obj can move to that position
    int r = pos.getRow();
    int c = pos.getCol();
    if (r < 0 || r >= this->num_rows || c < 0 || c >= this->num_cols) {
        return false;
    }

    //if the position is WALL
    MapElement * element = this->map[r][c];
    ElementType element_type = element->getType();
    if (element_type == WALL) {
        return false;
    }
    
    //if fake wall
    if (element_type == FAKE_WALL) {
        if (mv_obj->getName() == "Watson") {
            
            FakeWall * fakeWall = dynamic_cast<FakeWall*>(element);
            Character * character = dynamic_cast<Character*>(mv_obj);
            if (character->getExp() < fakeWall->getReqExp()) {
                return false;
            }
        }
    }
    return true;

}

void Map::printMap()
{
    MapElement *** map = this->map;
    for (int i = 0; i < this->num_rows; i++) {
        for (int j = 0; j < this->num_cols; j++) {
            if (this->map[i][j]->getType() == PATH) {
                cout << "*";
            }
            else if (this->map[i][j]->getType() == WALL) {
                cout << "W";
            }
            else if (this->map[i][j]->getType() == FAKE_WALL) {
                cout << "F";
            }
        }
        cout << endl;
    }
}

// bool Map::isValid(const Position& pos, MovingObject* mv_obj) {
//     //undermaintance
// }

//class Position
const Position Position::npos = Position(-1, -1);

Position::Position(const string & str_pos) {
    // (<r>, <c>)
    int r = 0;
    int c = 0;
    int i = 1;
    while (str_pos[i] != ',') {
        if ( str_pos[i] == ' '){
            i++;
            continue;
        }
        r = r * 10 + (str_pos[i] - '0');
        i++;
    }
    i++;
    // cout << c << "\n";
    while (str_pos[i] != ')') {
        if ( str_pos[i] == ' '){
            i++;
            continue;
        }
        c = c * 10 + (str_pos[i] - '0');
        i++;
    }

    this->r = r;
    this->c = c;
}

Position::Position(int r, int c) {
    this->r = r;
    this->c = c;
}


int Position::getRow() const {
    return this->r;
}

int Position::getCol() const {
    return this->c;
}

void Position::setRow(int r) {
    this->r = r;
}

void Position::setCol(int c) {
    this->c = c;
}

string Position::str() const {
    return "(" + to_string(this->r) + "," + to_string(this->c) + ")";
}

bool Position::isEqual(int r, int c) const {
    return (this->r == r && this->c == c);
}

bool Position::isEqual(const Position & pos) const {
    return (this->r == pos.r && this->c == pos.c);
}

//class MovingObject

MovingObject::MovingObject(int index, const Position pos, Map * map, const string & name) {
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = name;
}

MovingObject::~MovingObject() {
    // destructor
}

Position MovingObject::getCurrentPosition() const {
    return this->pos;
};

void MovingObject::setPosition(const Position & pos) {
    this->pos = pos;
}

void MovingObject::setMap(Map * map) {
    this->map = map;
}

int MovingObject::getIndex() const {
    return this->index;
}

string MovingObject::getName() const {
    return this->name;
}

void MovingObject::setName(const string & name) {
    this->name = name;
}

void MovingObject::setIndex(int index) {
    this->index = index;
}

//class Character

Character::Character(int index, const Position& init_pos, Map* map, int init_hp, int init_exp, const string& name, string moving_rule) : MovingObject(index, init_pos, map, name) {
    this->hp = init_hp;
    this->exp = init_exp;
    this->moving_rule = moving_rule;
    //constructor
}

Character::~Character() {
    //destructor
}

int Character::getHp() const {
    return this->hp > 500 ? 500 : this->hp;
}

int Character::getExp() const {
    return this->exp > 900 ? 900 : this->exp;
}

void Character::setHp(int hp)
{
    this->hp = hp;
}

void Character::setExp(int exp)
{
    this->exp = exp;
}

//class Sherlock

Sherlock::Sherlock(int index, const string& moving_rule, const Position& init_pos, Map* map, int init_hp, int init_exp) : Character(index, init_pos, map, init_hp, init_exp, "Sherlock", moving_rule){
    this->cur_moving_rule = moving_rule;
    //constructor
}

Position Sherlock::getNextPosition() {
    if (this->getHp() == 0){
        return this->pos.npos;
    }
    char current = this->cur_moving_rule[0];
    if (this->cur_moving_rule.length() > 1) {
        this->cur_moving_rule = this->cur_moving_rule.substr(1) + current; 
    }
    int r = this->pos.getRow();
    int c = this->pos.getCol();

    if (current == 'U') {
        r--;
    } else if (current == 'D') {
        r++;
    } else if (current == 'L') {
        c--;
    } else if (current == 'R') {
        c++;
    }

    if (this->map->isValid(Position(r, c), this)) {
        return Position(r, c);
    }
    else{
        return this->pos.npos;
    }
    
}

void Sherlock::move() {
    Position next = this->getNextPosition();
    // cout << "Sherlock::move()" << endl;
    if (next.isEqual(this->pos.npos)) {
        return;
    }
    this->pos = next;
}

string Sherlock::str() const {
    //Sherlock[index=<index>;pos=<pos>;moving_rule=<moving_rule>]
    return "Sherlock[index=" + to_string(this->index) + ";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]";
};

void Sherlock::sherlock_action(ArrayMovingObject * arr_mv_objs, SherlockBag * sherlock_bag, WatsonBag * watson_bag){

    int n = arr_mv_objs->size();

    for (int i = 0; i < n; i++){
        MovingObject * mv_obj = arr_mv_objs->get(i);
        if ( mv_obj->getCurrentPosition().isEqual(this->getCurrentPosition()) )
        {
            if (mv_obj->getName() == "Watson"){
                // If Watson have PassingCard and Sherlock have ExcemptionCard then they trade card by card 
                BaseItem * sherlock_card = sherlock_bag->get(PASSING_CARD);
                BaseItem * watson_card = watson_bag->get(PASSING_CARD);
                while (sherlock_card != nullptr && watson_card != nullptr){
                    
                    sherlock_bag->insert(watson_card);
                    watson_bag->insert(sherlock_card);
                
                    sherlock_card = sherlock_bag->get(PASSING_CARD);
                    watson_card = watson_bag->get(PASSING_CARD);

                }
            }
            
            if (mv_obj->getName() == "Robot"){
                Robot * robot = dynamic_cast<Robot*>(mv_obj);
                if (robot->getRobotType() == C){
                    if (this->getExp() < 500){
                        sherlock_bag->insert(robot->getItem());
                    }
                }
                else if (robot->getRobotType() == S){
                    if (this->getExp() > 400){
                        sherlock_bag->insert(robot->getItem());
                    }
                    else{
                        if (sherlock_bag->get(EXCEMPTION_CARD) == nullptr)
                            this->setExp(roundUp(this->getExp() * 0.9));
                    }
                }
                else if (robot->getRobotType() == W){
                    sherlock_bag->insert(robot->getItem());
                }
                else if (robot->getRobotType() == SW){
                    if (this->getExp() > 300 && this->getHp() > 335){
                        sherlock_bag->insert(robot->getItem());
                    }
                    else{
                        if (sherlock_bag->get(EXCEMPTION_CARD) == nullptr){
                            this->setExp(roundUp(this->getExp() * 0.85));
                            this->setHp(roundUp(this->getHp() * 0.85));
                        }
                    }
                }
            }

            // using item to buff
            BaseItem * item = sherlock_bag->get();
            if (item != nullptr){
                item->use(this);
            }

        }
    }
}

//class Watson 

Watson::Watson(int index, const string& moving_rule, const Position& init_pos, Map* map, int init_hp, int init_exp) : Character(index, init_pos, map, init_hp, init_exp, "Watson", moving_rule) {
    this->cur_moving_rule = moving_rule;
    //constructor
}

Position Watson::getNextPosition() {
    // cout << this->cur_moving_rule;
    if (this->getHp() == 0){
        return this->pos.npos;
    }
    char current = this->cur_moving_rule[0];
    if (this->cur_moving_rule.length() > 1) {
        this->cur_moving_rule = this->cur_moving_rule.substr(1) + current; 
    }
    
    int r = this->pos.getRow();
    int c = this->pos.getCol();

    // cout <<r << " " << c << "\n";
    // cout <<current << "\n";

    if (current == 'U') {
        r--;
    } else if (current == 'D') {
        r++;
    } else if (current == 'L') {
        c--;
    } else if (current == 'R') {
        c++;
    }

    // cout <<r << " " << c << "\n";
    if (this->map->isValid(Position(r, c), this)) {
        return Position(r, c);
    }
    else{
        return this->pos.npos;
    }
}

void Watson::move() {
    Position next = this->getNextPosition();
    if (next.isEqual(this->pos.npos.getRow(), this->pos.npos.getCol())) {
        return;
    }
    this->pos = next;
}

string Watson::str() const {
    //Watson[index=<index>;pos=<pos>;moving_rule=<moving_rule>]
    return "Watson[index=" + to_string(this->index) + ";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]";
}

void Watson::watson_action(ArrayMovingObject * arr_mv_objs, SherlockBag * sherlock_bag, WatsonBag * watson_bag){

    int n = arr_mv_objs->size();
    for (int i = 0; i < n ; i++){
        MovingObject * mv_obj = arr_mv_objs->get(i);
        if ( mv_obj->getCurrentPosition().isEqual(this->getCurrentPosition()) )
        {
            
            if (mv_obj->getName() == "Robot"){
                Robot * robot = dynamic_cast<Robot*>(mv_obj);
                if (robot->getRobotType() == C){
                    watson_bag->insert(robot->getItem());
                }
                else if (robot->getRobotType() == S){
                    // Do nothing 
                }
                else if (robot->getRobotType() == W){
                    BaseItem * passcard = watson_bag->get(PASSING_CARD);
                    if (passcard != nullptr){
                        passcard->use(this);
                        watson_bag->insert(robot->getItem());
                    }
                    else{
                        if (this->getHp() > 350){
                            watson_bag->insert(robot->getItem());
                        }
                        else{
                            this->setHp(roundUp(this->getHp() * 0.95));
                        }
                    }
                }
                else if (robot->getRobotType() == SW){
                    BaseItem * passcard = watson_bag->get(PASSING_CARD);
                    if (passcard != nullptr){
                        passcard->use(this);
                        watson_bag->insert(robot->getItem());
                    }
                    else{
                        if (this->getHp() > 165 && this->getExp() > 600){
                            watson_bag->insert(robot->getItem());
                        }
                        else{
                            this->setHp(roundUp(this->getHp() * 0.85));
                            this->setExp(roundUp(this->getExp() * 0.85));
                        }
                    }
                }
            }

            // using item to buff
            BaseItem * item = watson_bag->get();
            if (item != nullptr){
                item->use(this);
            }

        }
    }
}

//class Criminal
Criminal::Criminal(int index, const Position& init_pos, Map* map, Sherlock* sherlock, Watson* watson) : MovingObject(index, init_pos, map, "Criminal") {
    this->sherlock = sherlock;
    this->watson = watson;
    //constructor
}

Position Criminal::getNextPosition() {
    char mv[4]={ 'U', 'D', 'L', 'R' };
    int r = this->pos.getRow();
    int c = this->pos.getCol();

    Sherlock * s = this -> sherlock;
    Watson * w = this -> watson;

    int s_r = s -> getCurrentPosition().getRow();
    int s_c = s -> getCurrentPosition().getCol();
    int w_r = w -> getCurrentPosition().getRow();
    int w_c = w -> getCurrentPosition().getCol();

    int max_dist = 0;
    int t_r = Position::npos.getRow();
    int t_c = Position::npos.getCol();
    for (int i = 0; i < 4; i++) {
        int temp_r = r;
        int temp_c = c;
        if (mv[i] == 'U') {
            temp_r--;
        } else if (mv[i] == 'D') {
            temp_r++;
        } else if (mv[i] == 'L') {
            temp_c--;
        } else if (mv[i] == 'R') {
            temp_c++;
        }
        if (this->map->isValid(Position(temp_r, temp_c), this)) {
            int dist_s = abs(temp_r - s_r) + abs(temp_c - s_c);
            int dist_w = abs(temp_r - w_r) + abs(temp_c - w_c);
            if (dist_s > max_dist && dist_w > max_dist) {
                max_dist = dist_s;
                t_r = temp_r;
                t_c = temp_c;
            }
        }
    }
    return Position(t_r, t_c);
}

Position Criminal::getPreviousPosition() const {
    return this->previous_pos;
}

void Criminal::move() {
    Position next = this->getNextPosition();
    if (next.isEqual(this->pos.npos.getRow(), this->pos.npos.getCol())) {
        return;
    }
    this->previous_pos = this->pos;
    this->pos = next;
    this->nsteps += 1;
}

void Criminal::createRobot(ArrayMovingObject * arr_mv_objs) {
    if (this->nsteps % 3 == 0 && !arr_mv_objs->isFull()) {
        Position sherlock_pos = this->sherlock->getCurrentPosition();
        Position watson_pos = this->watson->getCurrentPosition();

        int r = this->pos.getRow();
        int c = this->pos.getCol();
        int s_r = sherlock_pos.getRow();
        int s_c = sherlock_pos.getCol();
        int w_r = watson_pos.getRow();
        int w_c = watson_pos.getCol();

        int Sdist = abs(r - s_r) + abs(c - s_c);
        int Wdist = abs(r - w_r) + abs(c - w_c);

        Robot * robot = nullptr;
        if (this->nsteps == 3){
            robot = new RobotC(arr_mv_objs->size(), this->pos, this->map, this);
        }
        else if (Sdist < Wdist){
            robot = new RobotS(arr_mv_objs->size(), this->pos, this->map, this, this->sherlock);
        }
        else if (Sdist > Wdist){
            robot = new RobotW(arr_mv_objs->size(), this->pos, this->map, this, this->watson);
        }
        else{
            robot = new RobotSW(arr_mv_objs->size(), this->pos, this->map, this, this->sherlock, this->watson);
        }

        arr_mv_objs->add(robot);

    }
}

string Criminal::str() const {
    //Criminal[index=<index>;pos=<pos>]
    return "Criminal[index=" + to_string(this->index) + ";pos=" + this->pos.str() + "]";
};

//class ArrayMovingObject
ArrayMovingObject::ArrayMovingObject(int capacity) {
    this->capacity = capacity;
    this->count = 0;
    this->arr_mv_objs = (MovingObject**)malloc(sizeof(MovingObject*) * capacity);
}

ArrayMovingObject::~ArrayMovingObject() {
    free(this->arr_mv_objs);
    this -> capacity = 0;
    this -> count = 0;
}

bool ArrayMovingObject::add(MovingObject * mv_obj) {
    if (this->count < this->capacity) {
        this->arr_mv_objs[mv_obj->getIndex()] = mv_obj;
        this->count++;
        return true;
    }
    else {
        return false;
    }
}

MovingObject *ArrayMovingObject::get(int index) const
{
    return this->arr_mv_objs[index];
}

bool ArrayMovingObject::isFull() const {
    return (this->count == this->capacity);
}

int ArrayMovingObject::size() const {
    return this->count;
}

string ArrayMovingObject::str() const {
    //ArrayMovingObject[count=<count>;capacity=<capacity>;<MovingObject1>;...]
    string result = "ArrayMovingObject[count=" + to_string(this->count) + ";capacity=" + to_string(this->capacity) ;
    for (int i = 0; i < this->count; i++) {
        result += ";" + this->arr_mv_objs[i]->str() ;
    }
    result += "]";
    return result;
}

//class Configuration 

Configuration::Configuration(const string & filepath){
    ifstream file(filepath);
    string line;
    /*1. MAP_NUM_ROWS=<nr>
2. MAP_NUM_COLS=<nc>
3. MAX_NUM_MOVING_OBJECTS=<mnmo>
4. ARRAY_WALLS=<aw>
5. ARRAY_FAKE_WALLS=<afw>
6. SHERLOCK_MOVING_RULE=<smr>
7. SHERLOCK_INIT_POS=<sip>
8. SHERLOCK_INIT_HP=<sih>
9. SHERLOCK_INIT_EXP=<exp>
10. WATSON_MOVING_RULE=<wmr>
11. WATSON_INIT_POS=<wip>
12. WATSON_INIT_HP=<wih>
13. WATSON_INIT_EXP=<wie>
14. CRIMINAL_INIT_POS=<cip>
15. NUM_STEPS=<ns>
*/
    while (getline(file, line)) {
        string key = line.substr(0, line.find('='));
        string value = line.substr(line.find('=') + 1);
        if (key == "MAP_NUM_ROWS") {
            this->map_num_rows = stoi(value);
        }
        else if (key == "MAP_NUM_COLS") {
            this->map_num_cols = stoi(value);
        }
        else if (key == "MAX_NUM_MOVING_OBJECTS") {
            this->max_num_moving_objects = stoi(value);
        }
        else if (key == "ARRAY_WALLS") {
            int temp = 0;
            for (int i = 0; i < value.length(); i++) {
                if (value[i] == ';') {
                    temp++;
                }
            }
            if (value == "[]"){
                this->num_walls = 0;
                this->arr_walls = nullptr;
            }
            else{
                this->num_walls = temp + 1;
                this->arr_walls = stringtoarray(value, this->num_walls);
            }
        }
        else if (key == "ARRAY_FAKE_WALLS") {
            int temp = 0;
            for (int i = 0; i < value.length(); i++) {
                if (value[i] == ';') {
                    temp++;
                }
            }
            if (value == "[]") {
                this->num_fake_walls = 0;
                this->arr_fake_walls = nullptr;
            }
            else{
                this->num_fake_walls = temp + 1;
                this->arr_fake_walls = stringtoarray(value, this->num_fake_walls);
            }
        }
        else if (key == "SHERLOCK_MOVING_RULE") {
            this->sherlock_moving_rule = value;
        }
        else if (key == "SHERLOCK_INIT_POS") {
            this->sherlock_init_pos = Position(value);
        }
        else if (key == "SHERLOCK_INIT_HP") {
            this->sherlock_init_hp = stoi(value);
        }
        else if (key == "SHERLOCK_INIT_EXP") {
            this->sherlock_init_exp = stoi(value);
        }
        else if (key == "WATSON_MOVING_RULE") {
            this->watson_moving_rule = value;
        }
        else if (key == "WATSON_INIT_POS") {
            this->watson_init_pos = Position(value);
        }
        else if (key == "WATSON_INIT_HP") {
            this->watson_init_hp = stoi(value);
        }
        else if (key == "WATSON_INIT_EXP") {
            this->watson_init_exp = stoi(value);
        }
        else if (key == "CRIMINAL_INIT_POS") {
            this->criminal_init_pos = Position(value);
        }
        else if (key == "NUM_STEPS") {
            this->num_steps = stoi(value);
        }
    }
}

Configuration::~Configuration(){
    free(this->arr_walls);
    free(this->arr_fake_walls);
    this -> map_num_rows = 0;
    this -> map_num_cols = 0;
    this -> max_num_moving_objects = 0;
    this -> num_walls = 0;
    this -> num_fake_walls = 0;
    this -> sherlock_init_hp = 0;
    this -> sherlock_init_exp = 0;
    this -> watson_init_hp = 0;
    this -> watson_init_exp = 0;
    this -> num_steps = 0;
}


string Configuration::str() const{
    /*Configuration[
<attribute_name1>=<attribute_value1>...
]*/
    stringstream ss;
    ss << "Configuration[" << endl;
    ss << "MAP_NUM_ROWS=" << map_num_rows << endl;
    ss << "MAP_NUM_COLS=" << map_num_cols << endl;
    ss << "MAX_NUM_MOVING_OBJECTS=" << max_num_moving_objects << endl;
    ss << "NUM_WALLS=" << num_walls << endl;
    ss << "ARRAY_WALLS=[";
    for (int i = 0; i < num_walls; i++) {
        ss << arr_walls[i].str();
        if (i != num_walls - 1) {
            ss << ";";
        }
    }
    ss << "]" << endl;
    ss << "NUM_FAKE_WALLS=" << num_fake_walls << endl;
    ss << "ARRAY_FAKE_WALLS=[";
    for (int i = 0; i < num_fake_walls; i++) {
        ss << arr_fake_walls[i].str();
        if (i != num_fake_walls - 1) {
            ss << ";";
        }
    }
    ss << "]" << endl;
    ss << "SHERLOCK_MOVING_RULE=" << sherlock_moving_rule << endl;
    ss << "SHERLOCK_INIT_POS=" << sherlock_init_pos.str() << endl;
    ss << "SHERLOCK_INIT_HP=" << sherlock_init_hp << endl;
    ss << "SHERLOCK_INIT_EXP=" << sherlock_init_exp << endl;
    ss << "WATSON_MOVING_RULE=" << watson_moving_rule << endl;
    ss << "WATSON_INIT_POS=" << watson_init_pos.str() << endl;
    ss << "WATSON_INIT_HP=" << watson_init_hp << endl;
    ss << "WATSON_INIT_EXP=" << watson_init_exp << endl;
    ss << "CRIMINAL_INIT_POS=" << criminal_init_pos.str() << endl;
    ss << "NUM_STEPS=" << num_steps << endl;
    ss << "]";
    return ss.str();

}


int Configuration::getNumRows() const {
    return map_num_rows;
}

int Configuration::getNumCols() const {
    return map_num_cols;
}

int Configuration::getMaxNumMovingObjects() const {
    return max_num_moving_objects;
}

int Configuration::getNumWalls() const {
    return num_walls;
}

Position* Configuration::getArrWalls() const {
    return arr_walls;
}

int Configuration::getNumFakeWalls() const {
    return num_fake_walls;
}

Position* Configuration::getArrFakeWalls() const {
    return arr_fake_walls;
}

string Configuration::getSherlockMovingRule() const {
    return sherlock_moving_rule;
}

Position Configuration::getSherlockInitPos() const {
    return sherlock_init_pos;
}

int Configuration::getSherlockInitHp() const {
    return sherlock_init_hp;
}

int Configuration::getSherlockInitExp() const {
    return sherlock_init_exp;
}

string Configuration::getWatsonMovingRule() const {
    return watson_moving_rule;
}

Position Configuration::getWatsonInitPos() const {
    return watson_init_pos;
}

int Configuration::getWatsonInitHp() const {
    return watson_init_hp;
}

int Configuration::getWatsonInitExp() const {
    return watson_init_exp;
}

Position Configuration::getCriminalInitPos() const {
    return criminal_init_pos;
}

int Configuration::getNumSteps() const {
    return num_steps;
};

//class robot

Robot::Robot(int index, const Position& init_pos, Map* map, Criminal* criminal) : MovingObject(index, init_pos, map, "Robot") {
    this -> criminal = criminal;
    //constructor
    //create item
    int r = this->pos.getRow();
    int c = this->pos.getCol();

    int p = r*c;
    while (p >= 10){
        int s = 0;
        while (p > 0){
            s += p % 10;
            p /= 10;
        }
        p = s;
    }
    if (p == 0 || p == 1){
        this->item = new MagicBook();
    }
    else if (p == 2 || p == 3){
        this->item = new EnergyDrink();
    }
    else if (p == 4 || p == 5){
        this->item = new FirstAid();
    }
    else if (p == 6 || p == 7){
        this->item = new ExcemptionCard();
    }
    else if (p == 8 || p == 9){
        int t = (r*11 + c)%4;
        if (t == 0){
            this->item = new PassingCard("RobotS");
        }
        else if (t == 1){
            this->item = new PassingCard("RobotC");
        }
        else if (t == 2){
            this->item = new PassingCard("RobotSW");
        }
        else if (t == 3){
            this->item = new PassingCard("all");
        }
    }
}

Robot::~Robot() {
    free(this->item);
    //destructor
}

RobotType Robot::getRobotType() const {
    return this->robot_type;
}

BaseItem* Robot::getItem() const {
    return this->item;
}

void Robot::setItem(BaseItem* item) {
    this->item = item;
}

void Robot::setRobot_type(RobotType robot_type) {
    this->robot_type = robot_type;
}

int Robot::getDistance(Sherlock * sherlock)const{
    int r = this->pos.getRow();
    int c = this->pos.getCol();
    int s_r = sherlock->getCurrentPosition().getRow();
    int s_c = sherlock->getCurrentPosition().getCol();
    return abs(r - s_r) + abs(c - s_c);
}

int Robot::getDistance(Watson * watson)const{
    int r = this->pos.getRow();
    int c = this->pos.getCol();
    int s_r = watson->getCurrentPosition().getRow();
    int s_c = watson->getCurrentPosition().getCol();
    return abs(r - s_r) + abs(c - s_c);
}

int RobotSW::getDistance()const{
    int r = this->pos.getRow();
    int c = this->pos.getCol();
    int s_r = this->sherlock->getCurrentPosition().getRow();
    int s_c = this->sherlock->getCurrentPosition().getCol();
    int w_r = this->watson->getCurrentPosition().getRow();
    int w_c = this->watson->getCurrentPosition().getCol();
    return abs(r - s_r) + abs(c - s_c) + abs(r - w_r) + abs(c - w_c);
}


RobotC::RobotC(int index, const Position& init_pos, Map* map, Criminal* criminal) : Robot(index, init_pos, map, criminal) {
    setRobot_type(C);
    //constructor
}

Position RobotC::getNextPosition() {
    return this->criminal->getPreviousPosition();
}

void RobotC::move() {
    Position next = this->getNextPosition();
    if (next.isEqual(this->pos.npos.getRow(), this->pos.npos.getCol())) {
        return;
    }
    this->pos = next;
}

string RobotC::str() const {
    //RobotC[index=<index>;pos=<pos>]
    return "Robot[pos=" +  this->pos.str() + ";type=C;dist=]";
}

RobotS::RobotS(int index, const Position& init_pos, Map* map, Criminal* criminal , Sherlock * sherlock) : Robot(index, init_pos, map, criminal) {
    setRobot_type(S);
    this->sherlock = sherlock;
    //constructor
}

Position RobotS::getNextPosition(){
    //clock circle ^ > v <
    int _y[4] = {0, 1, 0, -1};
    int _x[4] = {-1, 0, 1, 0};
    int r = this->pos.getRow();
    int c = this->pos.getCol();
    int s_r = this->sherlock->getCurrentPosition().getRow();
    int s_c = this->sherlock->getCurrentPosition().getCol();
    int maxdist = int(0xdeadc0de);
    int mindist = maxdist;
    int dox = 0, doy = 0;
    for (int i = 0; i < 4; i++) {
        int x = r + _x[i];
        int y = c + _y[i];
        if (this->map->isValid(Position(x, y), this)) {
            int dist = abs(x - s_r) + abs(y - s_c);
            if (dist < mindist) {
                mindist = dist;
                dox = x;
                doy = y;
            }
        }
    }   
    if (mindist == maxdist) {
        return this->pos.npos;
    }
    return Position(dox, doy);
}

void RobotS::move() {
    Position next = this->getNextPosition();
    if (next.isEqual(this->pos.npos.getRow(), this->pos.npos.getCol())) {
        return;
    }
    this->pos = next;
}

string RobotS::str() const {
    //RobotS[index=<index>;pos=<pos>]
    return "Robot[pos=" +  this->pos.str() + ";type=S;dist="+to_string(getDistance())+"]";
}

RobotW::RobotW(int index, const Position& init_pos, Map* map, Criminal* criminal, Watson * watson) : Robot(index, init_pos, map, criminal) {
    setRobot_type(W);
    this->watson = watson;
    //constructor
}

Position RobotW::getNextPosition() {
    //clock circle ^ > v <
    int _y[4] = {0, 1, 0, -1};
    int _x[4] = {-1, 0, 1, 0};
    int r = this->pos.getRow();
    int c = this->pos.getCol();
    int w_r = this->watson->getCurrentPosition().getRow();
    int w_c = this->watson->getCurrentPosition().getCol();
    int maxdist = int(0xdeadc0de);
    int mindist = maxdist;
    int dox = 0, doy = 0;
    for (int i = 0; i < 4; i++) {
        int x = r + _x[i];
        int y = c + _y[i];
        if (this->map->isValid(Position(x, y), this)) {
            int dist = abs(x - w_r) + abs(y - w_c);
            if (dist < mindist) {
                mindist = dist;
                dox = x;
                doy = y;
            }
        }
    }   
    if (mindist == maxdist) {
        return this->pos.npos;
    }
    return Position(dox, doy);
}

void RobotW::move() {
    Position next = this->getNextPosition();
    if (next.isEqual(this->pos.npos.getRow(), this->pos.npos.getCol())) {
        return;
    }
    this->pos = next;
}

string RobotW::str() const {
    //RobotW[index=<index>;pos=<pos>]
    return "Robot[pos=" +  this->pos.str() + ";type=W;dist="+to_string(getDistance())+"]";
}

RobotSW::RobotSW(int index, const Position& init_pos, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson) : Robot(index, init_pos, map, criminal) {
    setRobot_type(SW);
    this->sherlock = sherlock;
    this->watson = watson;
    //constructor
}

Position RobotSW::getNextPosition() {
    //clock circle ^ > v <
    int _y[8] = {0, 1, 2, 1, 0, -1, -2, -1};
    int _x[8] = {-2, -1, 0, 1, 2, 1, 0, -1};
    int r = this->pos.getRow();
    int c = this->pos.getCol();
    int s_r = this->sherlock->getCurrentPosition().getRow();
    int s_c = this->sherlock->getCurrentPosition().getCol();
    int w_r = this->watson->getCurrentPosition().getRow();
    int w_c = this->watson->getCurrentPosition().getCol();
    int maxdist = int(0xdeadc0de);
    int mindist = maxdist;
    int dox = 0, doy = 0;
    for (int i = 0; i < 4; i++) {
        int x = r + _x[i];
        int y = c + _y[i];
        if (this->map->isValid(Position(x, y), this)) {
            int dist = abs(x - s_r) + abs(y - s_c) + abs(x - w_r) + abs(y - w_c);
            if (dist < mindist) {
                mindist = dist;
                dox = x;
                doy = y;
            }
        }
    }   
    if (mindist == maxdist) {
        return this->pos.npos;
    }
    return Position(dox, doy);
}

void RobotSW::move() {
    Position next = this->getNextPosition();
    if (next.isEqual(this->pos.npos.getRow(), this->pos.npos.getCol())) {
        return;
    }
    this->pos = next;
}

string RobotSW::str() const {
    //RobotSW[index=<index>;pos=<pos>]
    return "Robot[pos=" +  this->pos.str() + ";type=SW;dist="+to_string(this->getDistance())+"]";
}

//Item
BaseItem::BaseItem(string item_name) {
    this->item_name = item_name;
}

// MagicBook
MagicBook::MagicBook(): BaseItem("MagicBook") {}

bool MagicBook::canUse(Character* character, Robot * robot) {
    if (character->getExp() <= 350) {
        return true;
    }
    return false;
}

void MagicBook::use(Character* character, Robot * robot) {
    int new_exp = roundUp(character->getExp() * 1.25);
    character->setExp(new_exp > 900 ? 900 : new_exp);
}

// EnergyDrink
EnergyDrink::EnergyDrink(): BaseItem("EnergyDrink") {}

bool EnergyDrink::canUse(Character* character, Robot * robot) {
    if (character->getHp() <= 100) {
        return true;
    }
    return false;
}

void EnergyDrink::use(Character* character, Robot * robot) {
    int new_hp = roundUp(character->getHp() * 1.2);
    character->setHp(new_hp > 500 ? 500 : new_hp);
}

// FirstAid 
FirstAid::FirstAid(): BaseItem("FirstAid") {}

bool FirstAid::canUse(Character* character, Robot * robot) {
    if (character->getHp() <= 100 || character->getExp() <= 350) {
        return true;
    }
    return false;
}

void FirstAid::use(Character* character, Robot * robot) {
    int new_hp = roundUp(character->getHp() * 1.5);
    character->setHp(new_hp > 500 ? 500 : new_hp);
}

//ExcemptionCard
ExcemptionCard::ExcemptionCard(): BaseItem("ExcemptionCard") {}

bool ExcemptionCard::canUse(Character* character, Robot * robot) {
    if (character -> getName() =="Sherlock" && (character-> getHp() % 2 == 1) )
    {
        return true;
    }
    return false;
}

void ExcemptionCard::use(Character* character, Robot * robot) {
    // not change the sherlock hp
}

// PassingCard
PassingCard::PassingCard(const string &challenge): BaseItem("PassingCard") {this -> challenge = challenge;}

bool PassingCard::canUse(Character* character, Robot * robot) {
    if (character -> getName() =="Watson" && (character-> getExp() % 2 == 0) )
    {
        return true;
    }
    return false;
}

void PassingCard::use(Character* character, Robot * robot) {
    bool check = (this -> challenge == "RobotS" && robot -> getRobotType() == S) || (this -> challenge == "RobotW" && robot -> getRobotType() != W) || (this -> challenge == "RobotC" && robot -> getRobotType() != C) || (this -> challenge == "RobotSW" && robot -> getRobotType() != SW);    
    if (check){
        character -> setExp(character -> getExp() - 50);
    }
}

//BaseBag

BaseBag::~BaseBag() {
    //destructor
    for (int i = 0; i < this->count; i++) {
        node* temp = this->head;
        this->head = this->head->next;
        delete temp;
    }
}

bool BaseBag::insert(BaseItem* item) {
    if (this->count < this->capacity) {
        node* new_node = new node(item);
        new_node->next = this->head;
        this->head = new_node;
        this->count++;
        return true;
    }
    return false;
}

BaseItem * BaseBag::get(){
    node * result = this->head;
    if (result == NULL){
        return NULL; //nah
    }
    if (this->count == 1)
    {
        if (result->item->canUse(this->obj)){
            this->head = NULL;
            this->count = 0;
            return result->item;
        }
        else{
            return NULL;
        }
    }
    else{
        //find the nearest item can use then move to the head and delete it
        node * prev = NULL;
        while (result != NULL){
            if (result->item->canUse(this->obj)){
                if (prev != NULL){
                    prev->next = result->next;
                }
                else{
                    this->head = result->next;
                }
                result->next = this->head;
                this->head = result;
                this->count--;
                return result->item;
            }
            prev = result;
            result = result->next;
        }
        return NULL;
    }
}

bool checkStrForItype(string str, ItemType itype){
    string itype_str[5] = {"MagicBook", "EnergyDrink", "FirstAid", "ExcemptionCard", "PassingCard"};

    return str == itype_str[itype];
}

BaseItem * BaseBag::get(ItemType itemType){
    node * result = this->head;
    if (result == NULL){
        return NULL; //nah
    }
    if (this->count == 1)
    {
        if (result->item->canUse(this->obj) && checkStrForItype(result->item->getItemName() , itemType) ){
            this->head = NULL;
            this->count = 0;
            return result->item;
        }
        else{
            return NULL;
        }
    }
    else{
        //find the nearest item can use then move to the head and delete it
        node * prev = NULL;
        while (result != NULL){
            if (result->item->canUse(this->obj) && checkStrForItype(result->item->getItemName() , itemType) ){
                if (prev != NULL){
                    prev->next = result->next;
                }
                else{
                    this->head = result->next;
                }
                result->next = this->head;
                this->head = result;
                this->count--;
                return result->item;
            }
            prev = result;
            result = result->next;
        }
        return NULL;
    }
}

string BaseBag::str() const {
    //BaseBag[count=<count>;capacity=<capacity>;<Item1>;...]
    string result = "Bag[count=" + to_string(this->count) + ";";
    node* current = this->head;
    while (current != NULL) {
        result += current->item->getItemName() + ";";
        current = current->next;
    }
    result += "]";
    return result;
}

BaseBag::BaseBag(Character* obj, int capacity) {
    this->obj = obj;
    this->capacity = capacity;
    this->count = 0;
    this->head = NULL;
}

SherlockBag::SherlockBag(Sherlock * sherlock) : BaseBag(sherlock, 13){
    //constructor
}

WatsonBag::WatsonBag(Watson * watson) : BaseBag(watson, 15){
    //constructor
}

//Study in pink ~ 
StudyPinkProgram::StudyPinkProgram(const string &config_file_path)
{
    this->config = new Configuration(config_file_path);
    //Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls)
    this->map = new Map(this->config->getNumRows(), this->config->getNumCols(), this->config->getNumWalls(), this->config->getArrWalls(), this->config->getNumFakeWalls(), this->config->getArrFakeWalls());
    this->sherlock = new Sherlock(1, this->config->getSherlockMovingRule(), this->config->getSherlockInitPos(), this->map, this->config->getSherlockInitHp(), this->config->getSherlockInitExp());
    this->watson = new Watson(2, this->config->getWatsonMovingRule(), this->config->getWatsonInitPos(), this->map, this->config->getWatsonInitHp(), this->config->getWatsonInitExp());
    this->criminal = new Criminal(3, this->config->getCriminalInitPos(), this->map, this->sherlock, this->watson);
    this->arr_mv_objs = new ArrayMovingObject(this->config->getMaxNumMovingObjects());
    this->sherlock_bag = new SherlockBag(sherlock);
    this->watson_bag = new WatsonBag(watson);

    this->arr_mv_objs->add(this->sherlock);
    this->arr_mv_objs->add(this->watson);
    this->arr_mv_objs->add(this->criminal);
}

StudyPinkProgram::~StudyPinkProgram() {
    delete this->config;
    delete this->map;
    delete this->sherlock;
    delete this->watson;
    delete this->criminal;
    delete this->arr_mv_objs;
    delete this->sherlock_bag;
    delete this->watson_bag;
    //destructor
}

bool StudyPinkProgram::isStop() const{
    // Sherlock and Watson are out of HP
    if (this->sherlock->getHp() <= 0 && this->watson->getHp() <= 0) {
        return true;
    }
    // Sherlock and Watson catch the Criminal
    if (this->sherlock->getCurrentPosition().isEqual(this->criminal->getCurrentPosition().getRow(), this->criminal->getCurrentPosition().getCol()) && this->watson->getCurrentPosition().isEqual(this->criminal->getCurrentPosition().getRow(), this->criminal->getCurrentPosition().getCol())) {
        return true;
    }

    // Sherlock catch RobotC and jump to the Criminal if Exp > 500
    RobotC * robotC = nullptr;
    for (int i = 0 ; i < this->arr_mv_objs->size(); i++){
        if (this->arr_mv_objs->get(i)->getName() == "Robot" && dynamic_cast<Robot*>(this->arr_mv_objs->get(i))->getRobotType() == C){
            robotC = dynamic_cast<RobotC*>(this->arr_mv_objs->get(i));
            if (robotC->getCurrentPosition().isEqual(this->sherlock->getCurrentPosition().getRow(), this->sherlock->getCurrentPosition().getCol()) && this->sherlock->getExp() > 500 ){
                this->sherlock->setPosition(this->criminal->getCurrentPosition());
                return true;
            }
        }
    }

    return false;

}

void StudyPinkProgram::run(bool verbose){
    // Note: This is a sample code. You can change the implementation as you like.
    // TODO
    for (int istep = 0; istep < config->getNumSteps(); ++istep) {
        for (int i = 0; i < arr_mv_objs->size(); ++i) {
            arr_mv_objs->get(i)->move();
            if (isStop()) {
                printStep(istep);
                break;
            }
            if (verbose) {
                printStep(istep);
            }

            if (arr_mv_objs->get(i)->getName() == "Criminal") {
                Criminal * criminal = dynamic_cast<Criminal*>(arr_mv_objs->get(i));
                criminal->createRobot(arr_mv_objs);
            }

            if (arr_mv_objs->get(i)->getName() == "Sherlock") {
                Sherlock * sherlock = dynamic_cast<Sherlock*>(arr_mv_objs->get(i));
                sherlock->sherlock_action(arr_mv_objs, sherlock_bag, watson_bag);
            }

            if (arr_mv_objs->get(i)->getName() == "Watson") {
                Watson * watson = dynamic_cast<Watson*>(arr_mv_objs->get(i));
                watson->watson_action(arr_mv_objs, sherlock_bag, watson_bag);
            }

            if (arr_mv_objs->get(i)->getName() == "Robot") {
                Robot * robot = dynamic_cast<Robot*>(arr_mv_objs->get(i));
                robot->move();
            }
        }   
    }
    printResult();
}


// ////////////////////////////////////////////////
// /// END OF STUDENT'S ANSWER
// ////////////////////////////////////////////////

