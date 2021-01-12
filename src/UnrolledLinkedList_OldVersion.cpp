//
// Created by Rainy Memory on 2021/1/12.
//

#include "UnrolledLinkedList_OldVersion.h"
bool Element::operator<(const Element &o) const {
    return strcmp(key, o.key) < 0;
}

Element::Element(int _offset, const string &_key) : offset(_offset) {
    memset(key, 0, sizeof(key));
    strcpy(key, _key.c_str());
}

Element &Element::operator=(const Element &o) {
    if (this == &o)return *this;
    offset = o.offset;
    memset(key, 0, sizeof(key));
    strcpy(key, o.key);
    return *this;
}

Block::Block() : next(-1), pre(-1), length(0) {

}

int UnrolledLinkedList::nextBlock(int offset) {
    fin_nextBlock.open(filename, ios::in | ios::binary);
    if (!fin_nextBlock)cerr << "[Error] File open failed in \"UnrolledLinkedList::nextBlock\"." << endl;
    fin_nextBlock.seekg(offset);
    int next;
    fin_nextBlock.read(reinterpret_cast<char *>(&next), sizeof(int));//"int next" is at the begin of class Block.
    fin_nextBlock.close();
    return next;
}

void UnrolledLinkedList::mergeBlock(int offset1, int offset2) {
    //presume offset1 < offset2, which means block 1 is before block 2
    //delete block 2
    
    fin.open(filename, ios::in | ios::binary);
    fout.open(filename, ios::in | ios::out | ios::binary);
    if ((!fin) | (!fout))cerr << "[Error] File open failed in \"UnrolledLinkedList::mergeBlock\"." << endl;
    
    //o1->next=o2->next;
    int next;
    fin.seekg(offset2);
    fin.read(reinterpret_cast<char *>(&next), sizeof(int));
    fout.seekp(offset1);
    fout.write(reinterpret_cast<const char *>(&next), sizeof(int));
    
    //o2->next->pre=o1;
    if (next >= 0) {
        fout.seekp(next + sizeof(int));
        fout.write(reinterpret_cast<const char *>(&offset1), sizeof(int));
    }
    
    int len1, len2;
    fin.seekg(offset1 + 2 * sizeof(int));
    fin.read(reinterpret_cast<char *>(&len1), sizeof(int));
    fin.seekg(offset2 + 2 * sizeof(int));
    fin.read(reinterpret_cast<char *>(&len2), sizeof(int));
    int tot_len = len1 + len2;
    fout.seekp(offset1 + 2 * sizeof(int));
    fout.write(reinterpret_cast<const char *>(&tot_len), sizeof(int));
    
    fin.seekg(offset2 + 3 * sizeof(int));
    fout.seekp(offset1 + 3 * sizeof(int) + len1 * sizeof(Element));
    Element temp;
    for (int i = 0; i < len2; i++) {
        fin.read(reinterpret_cast<char *>(&temp), sizeof(Element));
        fout.write(reinterpret_cast<const char *>(&temp), sizeof(Element));
    }
    
    fin.close();
    fout.close();
}

void UnrolledLinkedList::splitBlock(int offset) {
    fin.open(filename, ios::in | ios::binary);
    fout.open(filename, ios::in | ios::out | ios::binary);
    if ((!fin) | (!fout))cerr << "[Error] File open failed in \"UnrolledLinkedList::splitBlock\"." << endl;
    
    fin.seekg(0, ios::end);
    int temp = fin.tellg();
    Block tempBlock;
    //temp->pre=this
    tempBlock.pre = offset;
    //temp->next=this->next
    int this_next;
    fin.seekg(offset);
    fin.read(reinterpret_cast<char *>(&this_next), sizeof(int));
    tempBlock.next = this_next;
    //this->next->pre=temp
    if (this_next >= 0) {
        fout.seekp(this_next + sizeof(int));
        fout.write(reinterpret_cast<const char *>(&temp), sizeof(int));
    }
    //this->next=temp
    fout.seekp(offset);
    fout.write(reinterpret_cast<const char *>(&temp), sizeof(int));
    
    fin.seekg(offset + 3 * sizeof(int) + SPLIT_REMAIN * sizeof(Element));
    Element tempElement;
    for (int i = SPLIT_REMAIN; i < BLOCK_SIZE; i++) {
        fin.read(reinterpret_cast<char *>(&tempElement), sizeof(Element));
        tempBlock.array[i - SPLIT_REMAIN] = tempElement;
    }
    fout.seekp(offset + 2 * sizeof(int));
    int newlen = SPLIT_REMAIN;
    fout.write(reinterpret_cast<const char *>(&newlen), sizeof(int));
    tempBlock.length = BLOCK_SIZE - SPLIT_REMAIN;
    
    fout.seekp(temp);
    fout.write(reinterpret_cast<const char *>(&tempBlock), sizeof(Block));
    
    fin.close();
    fout.close();
}

void UnrolledLinkedList::findElement(const string &key, vector<int> &result) {
    fin.open(filename, ios::in | ios::binary);
    if (!fin)cerr << "[Error] File open failed in \"UnrolledLinkedList::findElement\"." << endl;
    
    char _key[MAX_KEY_LENGTH] = {0};
    for (int i = 0; i < key.length(); i++)_key[i] = key[i];
    
    fin.seekg(0);
    int cur = fin.tellg();
    int next = nextBlock(cur);
    Element nextElement;
    while (next != -1) {
        fin.seekg(next + 3 * sizeof(int));
        fin.read(reinterpret_cast<char *>(&nextElement), sizeof(Element));
        if (strcmp(_key, nextElement.key) <= 0) break;
        cur = next;
        next = nextBlock(next);
    }
    
    bool flag = true;
    while (flag && (cur >= 0)) {
        flag = false;
        int len;
        Element tempElement;
        fin.seekg(cur + 2 * sizeof(int));
        fin.read(reinterpret_cast<char *>(&len), sizeof(int));
        
        for (int i = 0; i < len; i++) {
            fin.seekg(cur + 3 * sizeof(int) + i * sizeof(Element));
            fin.read(reinterpret_cast<char *>(&tempElement), sizeof(Element));
            if (strcmp(_key, tempElement.key) < 0) break;
            if (strcmp(_key, tempElement.key) == 0) {
                result.push_back(tempElement.offset);
                if (i == len - 1) {//if head Element's key is equal to target, then find next Block
                    flag = true;
                    cur = nextBlock(cur);
                }
            }
        }
    }
    
    fin.close();
}

void UnrolledLinkedList::addElement(const Element &o) {
    fin.open(filename, ios::in | ios::binary);
    fout.open(filename, ios::in | ios::out | ios::binary);
    if ((!fin) | (!fout))cerr << "[Error] File open failed in \"UnrolledLinkedList::addElement\"." << endl;
    
    fin.seekg(0);
    int cur = fin.tellg();
    int next = nextBlock(cur);
    Element nextElement;
    while (next != -1) {
        fin.seekg(next + 3 * sizeof(int));
        fin.read(reinterpret_cast<char *>(&nextElement), sizeof(Element));
        if (o < nextElement)break;
        cur = next;
        next = nextBlock(next);
    }
    
    int len;
    Element tempElement;
    fin.seekg(cur + 2 * sizeof(int));
    fin.read(reinterpret_cast<char *>(&len), sizeof(int));
    int pos;
    bool flag = true;
    for (int i = 0; i < len; i++) {
        fin.read(reinterpret_cast<char *>(&tempElement), sizeof(Element));
        if (o < tempElement) {
            pos = i;
            flag = false;
            break;
        }
    }
    if (flag)pos = len;
    for (int i = len - 1; i >= pos; i--) {
        fin.seekg(cur + 3 * sizeof(int) + i * sizeof(Element));
        fin.read(reinterpret_cast<char *>(&tempElement), sizeof(Element));
        fout.seekp(cur + 3 * sizeof(int) + (i + 1) * sizeof(Element));
        fout.write(reinterpret_cast<const char *>(&tempElement), sizeof(Element));
    }
    fout.seekp(cur + 3 * sizeof(int) + pos * sizeof(Element));
    fout.write(reinterpret_cast<const char *>(&o), sizeof(Element));
    
    len++;
    fout.seekg(cur + 2 * sizeof(int));
    fout.write(reinterpret_cast<char *>(&len), sizeof(int));
    
    fin.close();
    fout.close();
    
    if (len == BLOCK_SIZE)splitBlock(cur);
}

void UnrolledLinkedList::deleteElement(const Element &o) {
    fin.open(filename, ios::in | ios::binary);
    fout.open(filename, ios::in | ios::out | ios::binary);
    if ((!fin) | (!fout))cerr << "[Error] File open failed in \"UnrolledLinkedList::deleteElement\"." << endl;
    
    fin.seekg(0);
    int cur = fin.tellg();
    int next = nextBlock(cur);
    Element nextElement;
    while (next != -1) {
        fin.seekg(next + 3 * sizeof(int));
        fin.read(reinterpret_cast<char *>(&nextElement), sizeof(Element));
        if (strcmp(o.key, nextElement.key) <= 0)break;
        cur = next;
        next = nextBlock(next);
    }
    
    int pos = -1;
    int len;
    Element tempElement;
    while (pos < 0 && cur >= 0) {
        fin.seekg(cur + 2 * sizeof(int));
        fin.read(reinterpret_cast<char *>(&len), sizeof(int));
        for (int i = 0; i < len; i++) {
            fin.read(reinterpret_cast<char *>(&tempElement), sizeof(Element));
            if ((strcmp(tempElement.key, o.key) == 0) && (tempElement.offset == o.offset)) {
                pos = i;
                break;
            }
        }
        if (pos < 0)cur = nextBlock(cur);
    }

#ifdef debug
    if (pos < 0) {
        cerr << "[debug] cannot find element!" << endl;
        cerr << "key: " << o.key << " offset: " << o.offset << endl;
        fin.close();
        fout.close();
        return;
    }
#endif
    
    for (int i = pos; i < len; i++) {
        fin.seekg(cur + 3 * sizeof(int) + (i + 1) * sizeof(Element));
        fin.read(reinterpret_cast<char *>(&tempElement), sizeof(Element));
        fout.seekp(cur + 3 * sizeof(int) + i * sizeof(Element));
        fout.write(reinterpret_cast<const char *>(&tempElement), sizeof(Element));
    }
    
    len--;
    fout.seekp(cur + 2 * sizeof(int));
    fout.write(reinterpret_cast<char *>(&len), sizeof(int));
    
    next = nextBlock(cur);
    int nextLen;
    if (next >= 0) {
        fin.seekg(next + 2 * sizeof(int));
        fin.read(reinterpret_cast<char *>(&nextLen), sizeof(int));
    }
    
    fin.close();
    fout.close();
    
    if (next >= 0 && ((len + nextLen) <= BLOCK_SIZE))mergeBlock(cur, next);
}

#ifdef debug
void UnrolledLinkedList::printList() {
    fin.open(filename, ios::in | ios::binary);
    fout.open(filename, ios::in | ios::out | ios::binary);
    if ((!fin) | (!fout))cerr << "[Error] File open failed in \"UnrolledLinkedList::printList\"." << endl;
    
    cout<<"[debug] UnrolledLinkedList----------\\/"<<endl;
    fin.seekg(0);
    int cur = fin.tellg();
    int next=nextBlock(cur);
    Block tempBlock;
    int cnt=0,tot_len=0;
    while(cur>=0){
        fin.seekg(cur);
        fin.read(reinterpret_cast<char *>(&tempBlock),sizeof(Block));
        cout<<"[debug] Block   "<<cnt++<<":"<<endl;
        cout<<"[debug] next:   "<<tempBlock.next<<endl;
        cout<<"[debug] cur:    "<<cur<<endl;
        cout<<"[debug] pre:    "<<tempBlock.pre<<endl;
        cout<<"[debug] length: "<<tempBlock.length<<endl;
        tot_len+=tempBlock.length;
        for(int i=0;i<tempBlock.length;i++){
            cout<<"[debug] Element "<<i<<":   offset = "<<tempBlock.array[i].offset<<"\t\t\tkey = "<<tempBlock.array[i].key<<endl;
        }
        cout<<endl;
        cur=next;
        next=nextBlock(cur);
    }
    cout<<"[debug] totalLength: "<<tot_len<<endl;
    cout<<"[debug] UnrolledLinkedList----------/\\"<<endl;
    cout<<endl;
    
    fin.close();
    fout.close();
}
#endif

