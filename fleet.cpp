#include "fleet.h"
Fleet::Fleet()
        :m_root(nullptr)
{
    //done
}
Fleet::~Fleet(){
    //done
    clear();
}

void Fleet::insert(const Ship& ship) {
    //done
    if (MINID <= ship.getID() && ship.getID() <= MAXID) {
        m_root = r_insert(ship, m_root);
    }
}

void Fleet::clear(){
    //done
    r_clear(m_root);
    m_root = nullptr; //setting pointer itself to null
}

void Fleet::remove(int id){
    //done
    m_root = r_remove(id, m_root);
}

void Fleet::updateHeight(Ship* aShip){
    //done
    if (aShip == nullptr){
        return;
    }
    int right = -1;
    int left = -1;
    if (aShip->getLeft() != nullptr){
        left = aShip->getLeft()->getHeight();
    }
    if (aShip->getRight() != nullptr){
        right = aShip->getRight()->getHeight();
    }
    if (left < right){
        aShip->setHeight(right + 1);
    }
    else{
        aShip->setHeight(left + 1);
    }
}

int Fleet::checkImbalance(Ship* aShip){
    //done
    if (aShip == nullptr){ //if tree is empty
        return 0;
    }
    int leftHeight = -1;
    int rightHeight = -1;
    if (aShip->getRight() != nullptr){
        rightHeight = aShip->getRight()->getHeight();
    }
    if (aShip->getLeft() != nullptr){
        leftHeight = aShip->getLeft()->getHeight();
    }
    return rightHeight - leftHeight; //returning the balance factor
}

Ship* Fleet::rebalance(Ship* aShip) {
    //done
    if (aShip == nullptr) { //base
        return nullptr;
    }
    int b_factor = checkImbalance(aShip);
    if (b_factor < -1) {
        b_factor = checkImbalance(aShip->getLeft());
        if (b_factor < 0) { // LL case
        }
        else {  // LR case
            aShip->setLeft(l_rotate(aShip->getLeft()));
        }
        aShip = r_rotate(aShip);
    }
    else if (b_factor > 1) {
        b_factor = checkImbalance(aShip->getRight());
        if (b_factor > 0) { // RR case
        }
        else {  // RL case
            aShip->setRight(r_rotate(aShip->getRight()));
        }
        aShip = l_rotate(aShip);
    }
    return aShip;
}

void Fleet::dumpTree() const {
    dump(m_root);
    //done
}

void Fleet::dump(Ship* aShip) const{
    //done
    if (aShip != nullptr){
        cout << "(";
        dump(aShip->m_left);//first visit the left child
        cout << aShip->m_id << ":" << aShip->m_height;//second visit the node itself
        dump(aShip->m_right);//third visit the right child
        cout << ")";
    }
}
void Fleet::listShips() const{
    //done
    r_list(m_root);
}

bool Fleet::setState(int id, STATE state){
    //done
    return r_setState(id, state, m_root);
}

void Fleet::removeLost(){
    //done
    m_root = r_removeLost(m_root);
}

bool Fleet::findShip(int id) const {
    //done
    return r_findShip(id, m_root);
}

void Fleet::r_clear(Ship* temp){
    //done
    if (temp == nullptr){
        return;
    }
    r_clear(temp->getLeft()); //recursive call
    temp->setLeft(nullptr); //set ptr to null
    r_clear(temp->getRight()); //recursive call
    temp->setRight(nullptr); //set ptr to null
    delete temp;
    temp = nullptr; //keeping valgrind happy
}

Ship* Fleet::r_rotate(Ship* temp){ //right rotate function
    Ship* ptr = temp;
    temp = ptr->getLeft();
    ptr->setLeft(temp->getRight());
    temp->setRight(ptr);
    updateHeight(ptr);
    updateHeight(temp);
    ptr = nullptr;
    return temp;
}

Ship* Fleet::l_rotate(Ship* temp){
    Ship* ptr = temp;
    temp = ptr->getRight();
    ptr->setRight(temp->getLeft());
    temp->setLeft(ptr);
    updateHeight(ptr);
    updateHeight(temp);
    ptr = nullptr;
    return temp;
}

bool Fleet::r_findShip(int id, Ship* temp) const{ //const cuz called from a const function
    if (temp == nullptr){
        return false;
    }
    if (id < temp->getID()){
        return r_findShip(id, temp->getLeft());
    }
    if (temp->getID() < id){
        return r_findShip(id, temp->getRight());
    }
    return true;
}

Ship* Fleet::r_insert(const Ship& ship, Ship* root){
    //done
    if (root == nullptr){ //base case
        return new Ship(ship.getID(), ship.getType(), ship.getState());
    }
    if (ship.getID() < root->getID()){
        root->setLeft(r_insert(ship, root->getLeft()));
    }
    else if (root->getID() < ship.getID()){
        root->setRight(r_insert(ship, root->getRight()));
    }
    else{ //if id r same
    }
    updateHeight(root);
    return rebalance(root);
}

void Fleet::r_list(const Ship* temp) const{
    //done
    if (temp == nullptr){ //base case
        return;
    }
    r_list(temp->getLeft());
    cout << endl << temp->getID() << ":" << temp->getStateStr() << ":" << temp->getTypeStr();
    r_list(temp->getRight());
}

Ship* Fleet::r_remove(int id, Ship* root){
    //done
    if (root == nullptr){
        return nullptr;
    }
    if (id < MINID || id > MAXID){
        return nullptr;
    }
    if (id < root->getID()){ //means on left side
        root->setLeft(r_remove(id, root->getLeft()));
    }
    else if (id > root->getID()){ //means right side
        root->setRight(r_remove(id, root->getRight()));
    }
    else { //if id matches
        root = remove_root(root);
    }
    updateHeight(root);
    return rebalance(root);
}

Ship* Fleet::remove_root(Ship* root){
    //done
    Ship* min_node = nullptr;
    if (root->getRight() == nullptr){
        min_node = root->getLeft();
    }
    else{
        root->setRight(r_remove_min(root->getRight(), min_node));
        min_node->setLeft(root->getLeft()); //;eft child of subtree becomign mins child
        min_node->setRight(root->getRight());
        root->setRight(nullptr);
    }
    root->setLeft(nullptr);
    delete root;
    root = min_node;
    min_node = nullptr;
    updateHeight(root);
    return rebalance(root);
}

Ship* Fleet::r_remove_min(Ship* root, Ship*& min_ship){
    //done
    if (root->getLeft() == nullptr){
        min_ship = root;
        root = min_ship->getRight();
        min_ship->setRight(nullptr);
        min_ship->setHeight(0);
    }
    else{
        root->setLeft(r_remove_min(root->getLeft(), min_ship)); //recursve call
        updateHeight(root);
        root = rebalance(root);
    }
    return root;
}

bool Fleet::r_setState(int id, STATE state, Ship* root){
    //done
    if (root == nullptr){
        return false;
    }
    if (id < root->getID()){
        return r_setState(id, state, root->getLeft());
    }
    if (id > root->getID()){
        return r_setState(id, state, root->getRight());
    }
    root->setState(state);
    return true;
}

Ship* Fleet::r_removeLost(Ship* root) {
    if (root == nullptr){ //base case
        return nullptr;
    }
    root->setLeft(r_removeLost(root->getLeft()));
    root->setRight(r_removeLost(root->getRight()));
    if (root->getState() == LOST){
        root = remove_root(root);
    }
    return root;
}

bool Fleet::verifyAVL()
{
    int min_id, max_id;
    return r_verifyHeight(m_root) &&
           r_verifyBalance(m_root) &&
           r_verifyBST(m_root, min_id, max_id);
}

bool Fleet::r_verifyHeight(Ship* ship)
{
    if (ship == nullptr) {  // empty tree base case
        return true;
    }

    Ship* pLeft = ship->getLeft();
    Ship* pRight = ship->getRight();

    if (!r_verifyHeight(pLeft) || !r_verifyHeight(pRight)) {
        return false;
    }

    int rightHt = -1;
    int leftHt = -1;

    if (pRight != nullptr) {
        rightHt = pRight->getHeight();
    }
    if (pLeft != nullptr) {
        leftHt = pLeft->getHeight();
    }
    if (leftHt < rightHt) {
        if (ship->getHeight() == rightHt + 1) {
            return true;
        }
    }
    else {
        if (ship->getHeight() == leftHt + 1) {
            return true;
        }
    }

    return false;
}

bool Fleet::r_verifyBalance(Ship* ship)
{
    if (ship == nullptr) {
        return true;
    }
    if (!r_verifyBalance(ship->getLeft()) || !r_verifyBalance(ship->getRight())) {
        return false;
    }
    int bf = checkImbalance(ship);
    if (bf < -1 || bf > 1) {
        return false;
    }
    return true;
}

bool Fleet::r_verifyBST(Ship* ship, int& minid, int& maxid)
{
    // base cases: empty tree, left node, internal node with one child
    if (ship == nullptr) {
        return true;
    }
    Ship* pLeft = ship->getLeft();
    Ship* pRight = ship->getRight();
    int id = ship->getID();
    if (pLeft == nullptr && pRight == nullptr) {
        minid = id;
        maxid = id;
        return true;
    }
    if (pLeft == nullptr && pRight != nullptr) {
        minid = id;
        maxid = pRight->getID();
        return minid < maxid;
    }
    if (pLeft != nullptr && pRight == nullptr) {
        minid = pLeft->getID();
        maxid = id;
        return minid < maxid;
    }

    int maxidL, minidR;
    if (!r_verifyBST(pLeft, minid, maxidL)
        || !r_verifyBST(pRight, minidR, maxid)) {
        return false;
    }
    if (id <= maxidL) {
        return false;
    }
    if (minidR <= id) {
        return false;
    }
    return true;
}

int Fleet::getSize()
{
    return r_getSize(m_root);
}

int Fleet::r_getSize(Ship* ship)
{
    if (ship == nullptr) {
        return 0;
    }
    return 1 + r_getSize(ship->getLeft()) + r_getSize(ship->getRight());
}


