//
//  parser.cpp
//  lab3
//
//  Modified by Tarek Abdelrahman on 2020-10-04.
//  Created by Tarek Abdelrahman on 2018-08-25.
//  Copyright © 2018-2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.


#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
#include "Shape.h"

// This is the shape array, to be dynamically allocated
Shape** shapesArray;

// The number of shapes in the database, to be incremented 
int shapeCount = 0;

// The value of the argument to the maxShapes command
int max_shapes;

// ECE244 Student: you may want to add the prototype of
// helper functions you write here
void createErrCheck(string,stringstream&,stringstream&);
void createShape(string n,string t,int x_loc,int y_loc,int x_sz, int y_sz);
void moveErrCheck(string,stringstream&);
void checkRotate(string,stringstream&,stringstream&);
void draw(string,stringstream&);
void deleteShape(string,stringstream&);
bool checkMaxShapes(string);
int main() {

    string line;
    string command;
    
    cout << "> ";         // Prompt for input
    getline(cin, line);    // Get a line from standard input
    int oldMaxShapes;//use to keep track of the previous input value for max shape
    while (!cin.eof ()) {
        // Put the line in a linestream for parsing
        // Making a new sstream for each line so flags etc. are cleared
        stringstream lineStream (line);
        
        // Read from string stream into the command
        // The only way this can fail is if the eof is encountered
        lineStream >> command;
        // Check for the command and act accordingly
        // ECE244 Student: Insert your code here
        // make a new variable to store the oldmaxShapes since the shapesArray has the size of the previous input 
        //command maxShapes value
        // used to keep track of previous max shape
        if (command=="maxShapes"){
            //check error for command maxShapes
            bool error;
            string mLine=line;
            error=checkMaxShapes(mLine); //should have a return type telling if error free or not
            //if no error on maxShapes command
            if(error==false){
                lineStream >> max_shapes;
                //cout<<"maxshapes"<<max_shapes<<endl;
            
                if (shapesArray!= NULL)
                {   //new maxShape command is given
                    //if shapesArray is pointing to something,delete before assigning new 
                    //delete shape objects first then array
                    //shapeCount reset back to 0
                    //cout<<"not null"<<endl;
                    //cout<<"Oldmaxshape"<<oldMaxShapes<<endl;
                    for (int i = 0; i<oldMaxShapes; i++){  //the array size==previous value
                        if ((shapesArray[i])!= NULL){
                            delete shapesArray[i];
                            shapesArray[i]=nullptr;
                        }
                    }
                    delete [] shapesArray;
                    shapesArray = nullptr;
                    //cout<<shapeCount<<endl;
                    shapeCount=0;
                }
                shapesArray = new Shape*[max_shapes]; //allocate array of pointers dynamically
                for (int i = 0; i<max_shapes;i++)
                    shapesArray[i]= nullptr;
                cout<<"New database: max shapes is "<<max_shapes<<endl;
                string value=line.erase(0,9);
                stringstream sizeArr (value);
                sizeArr>>oldMaxShapes; //keep track of the previous input
            }
        }
        
        //command create name type loc loc size size
        else if(command=="create"){
            //create a subline to count the number of arguments input by user 
            string subline=line.erase(0,6); //copy constructor for string class
            stringstream newLine (subline);
            createErrCheck(subline,newLine,lineStream);      
        }
        
        //command move name loc loc
        else if(command=="move"){
            //stringstream cannot be copied
            //lineStream's command is read
            //copy string line and erase the command 
            string subMove=line.erase(0,4); //.erase() member of string class 
            //cout<<subMove<<endl;
            //error check for move command
            moveErrCheck(subMove,lineStream);   
        }
        
        //command rotate name angle
        //check if the shape name can be found or not x
        //then check if the angle is valid argument or not and check if is in the range of  0-360 x
        //too many arguments
        //too few arguments
        else if(command=="rotate"){
            string sline=line.erase(0,6);
            stringstream subline (sline);
            //check errors 
            checkRotate(sline, subline,lineStream);   
        }
        //command draw name check if name can be found 
        //too many arguments
        //too few arguments
        //command draw all
        //too many and too few arguments
        //display all the objects pointed by the shape pointers in the shapesArray
        else if(command=="draw"){
            string drawline=line.erase(0,4);
            draw(drawline,lineStream);      
        }
        //command delete name
        else if(command=="delete"){
            string subDelete=line.erase(0,6);
            deleteShape(subDelete,lineStream);    
        }
        
        //invalid command
        else{
            cout<<"Error: invalid command"<<endl;
        }
            
      
        
        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);
        command="   "; //clear command after every input;
    }  // End input loop until EOF.
    
    return 0;
}

//check errors for the command maxShapes
bool checkMaxShapes(string maxLine){
    string copyMaxLine=maxLine.erase(0,9); 
    string max=copyMaxLine;
    stringstream mline(max);
    stringstream mlinex(max);
    stringstream mline2(copyMaxLine);
    string argument;
    int value;
    int numOfArg=0;
    bool error=false;
    //cout<<max<<endl;
    while(mline2>>argument){
        numOfArg++;
    } //count the number of arguments input by user
    //cout<<numOfArg<<endl;
    //value exists while argument >=1
    char next;
    int k;
    if(numOfArg>=1){
       mline>>value;
       mlinex>>k;
       //cin.peek()=-1 if eof is the next char
       
        next=mlinex.peek(); //check the char after the value to ensure proper error being
        //cout<<next<<endl;
        //next is a char so .empty()do not apply here 
        const char* b=" ";
        
       //cout<<next<<endl;
        if(mline.fail()||(next!=*b && next!=EOF)){   //if not valid argument not correct type fail flag raised
            cout<<"Error: invalid argument"<<endl;
            error=true;
            return error; //error found exit the function
        }
        else{           //valid argument,check value
            if(value<0){
                cout<<"Error: invalid value"<<endl;
                error=true;
                return error; //error found exit the function
            }
        }
    }
    
    //too many arguments
    if(numOfArg>1){
        cout<<"Error: too many arguments"<<endl;
        error=true;
        return error;
     }
    
    //too few arguments
    if(numOfArg<1){
        cout<<"Error: too few arguments"<<endl;
        error=true;
        return error;
     }
    
    
    
}







//if the name can be found 
//too many and too few arguments
//command delete all
//delete all the objects 
//if u choose delete all,after u delete all, the array elements can be used again 
//if u delete one shape shapeCount does not change 
//array full while shapeCount == max_shapes
//that spot can not be reused again 
void deleteShape(string subDelete,stringstream&lineStream){
    string del2=subDelete;
    stringstream delete1 (subDelete);
    stringstream delete2 (del2);
    string name;
    string argument;
    string testname;
    int numOfArg=0;
    string all="all";
    while(delete1>>argument){
        numOfArg++;
    }//count the number of arguments one string stream is consumed
    //if arg>=1, name exists
    if(numOfArg>=1){
        delete2>>testname; //read in the argument to see if it is name or all
        if(testname!=all)
                lineStream>>name;
        else{
            
            //check if there is more than one argument for all
            //while delete all is chose, free all the memory allocation pointed by
            //the array element (only those are not nullptr) 
            if(numOfArg==1){
                for(int i =0; i<max_shapes;i++){
                    if(shapesArray[i]!=nullptr){
                        delete shapesArray[i];
                        shapesArray[i]=nullptr;
                    }
                }
                shapeCount=0;
                cout<<"Deleted: all shapes"<<endl;
                return;
            }
        }
        
    }
    
    //check if the name is read in successfully
    if(!name.empty()){
    //check if shape name can be found or not
    //check if shape name exists
        bool nameExist= false;
        for (int i = 0; i < max_shapes; i++){
            if(shapesArray[i]!=nullptr){ //as long a pointer points to an object
                if(name==(shapesArray[i])->getName()){ //extract its name and compare
                    nameExist=true;//name exists
                    break;
                             //found name exists,break the loop
                }  
             //if no shape object exists, the for loop will be skipped
            }
        }
        if(!nameExist){
            cout<<"Error: shape "<<name<<" not found"<<endl;//error found
            return; //exit the function
        }   
    }
    
    //too many arguments
    if(numOfArg>1){
        cout<<"Error: too many arguments"<<endl;
        return;
     }
    
    //too few arguments
    if(numOfArg<1){
        cout<<"Error: too few arguments"<<endl;
        return;
     }
    //cout<<"fails here?"<<endl;
    //if no error with name is the argument
    for(int i =0; i<max_shapes;i++){    //the pointer maybe a null, thus can't call its member function getname
        if(shapesArray[i]!=nullptr){    //the delete object maybe in a random location 
            if(name==shapesArray[i]->getName()){ //find the location of the name
                delete shapesArray[i];       //free the specific memory block
                shapesArray[i]=nullptr;      //set the pointer to null
                break;                       //jump out of the loop
            }
        }
    }
    cout<<"Deleted shape "<<name<<endl;
    
    
    
    
}
//command draw name check if name can be found 
//too many arguments
//too few arguments
//command draw all
//too many and too few arguments
//display all the objects pointed by the shape pointers in the shapesArray
void draw(string dline,stringstream&lineStream){
    string dline2=dline; //copy constructor of string
    stringstream drawline(dline);
    stringstream drawline2(dline2);
    string name;
    string argument;
    string testname;
    int numOfArg=0;
    string all="all";
    while(drawline>>argument){
        numOfArg++;
    }//count the number of arguments one string stream is consumed
    //cout<<numOfArg<<endl;
    //if arg>=1, name exists
    if(numOfArg>=1){
        drawline2>>testname; //read in the argument to see if it is name or all
        if(testname!=all)
                lineStream>>name;
        else{
            cout<<"Drew all shapes"<<endl;
            for(int i=0;i<max_shapes;i++){
                if(shapesArray[i]!=nullptr){
                    shapesArray[i]->draw();
                }
            }
            return;
        }
    }
    
    //check if the name is read in successfully
    if(!name.empty()){
    //check if shape name can be found or not
    //check if shape name exists
        bool nameExist= false;
        for (int i = 0; i < max_shapes; i++){
            if(shapesArray[i]!=nullptr){ //as long a pointer points to an object
                if(name==(shapesArray[i])->getName()){ //extract its name and compare
                    nameExist=true;//name exists
                    break;
                             //found name exists,break the loop
                }  
             //if no shape object exists, the for loop will be skipped
            }
        }
        if(!nameExist){
            cout<<"Error: shape "<<name<<" not found"<<endl;//error found
            return; //exit the function
        }   
    }
    
    //too many arguments
    if(numOfArg>1){
        cout<<"Error: too many arguments"<<endl;
        return;
     }
    
    //too few arguments
    if(numOfArg<1){
        cout<<"Error: too few arguments"<<endl;
        return;
     }
    
    //if no error occurs
    //call the draw function
    for(int i=0; i<max_shapes;i++){
        if(name==shapesArray[i]->getName()){
            cout<<"Drew ";
            shapesArray[i]->draw();
            break;
        }
    }
    
}
void checkRotate(string subRotate, stringstream&subline,stringstream&lineStream){
    string name;
    int angle;
    string arguments;
    int numOfArg=0;
    stringstream sline(subRotate);
    string testN;
    sline>>testN; //assume reading in the name successfully
    
    //count the number of arguments
    while(subline>>arguments){
        numOfArg++;
    }
    
    //if arg>=1, name exists
    if(numOfArg>=1){
        lineStream>>name;
    }
    //check if the name is read in successfully
    if(!name.empty()){
    //check if shape name can be found or not
    //check if shape name exists
        bool nameExist= false;
        for (int i = 0; i < max_shapes; i++){
            if(shapesArray[i]!=nullptr){ //as long a pointer points to an object
                if(name==(shapesArray[i])->getName()){ //extract its name and compare
                    nameExist=true;//name exists
                    break;
                             //found name exists,break the loop
                }  
             //if no shape object exists, the for loop will be skipped
            }
        }
        if(!nameExist){
            cout<<"Error: shape "<<name<<" not found"<<endl;//error found
            return; //exit the function
        }   
    }
    //next check second argument angle 
    //second argument angle exists while arguments>=2 x 
    //invalid argument or not x
    //value valid or not(check if is in the range of  0-360)
    char next; 
    int k;
    if(numOfArg>=2){
       sline>> k; //read in angle from second string stream 
       next=sline.peek();
       const char* b=" ";
       lineStream>>angle;
                               //account for decimal(36.5) and (36b)
        if(lineStream.fail()||(next!=*b && next!=EOF)){   //if not valid argument not correct type fail flag raised
            cout<<"Error: invalid argument"<<endl;
            return; //error found exit the function
        }
        else{           //valid argument,check value
            if(angle<0 || angle >360){
                cout<<"Error: invalid value"<<endl;
                return; //error found exit the function
            }
        }
    }
    //too many arguments
    if(numOfArg>2){
        cout<<"Error: too many arguments"<<endl;
        return;
     }
    
    //too few arguments
    if(numOfArg<2){
        cout<<"Error: too few arguments"<<endl;
        return;
     }
    
    //if no error occurs
    //call class function
    for(int i=0; i<max_shapes;i++){
        if(shapesArray[i]!=nullptr){
            if(name==shapesArray[i]->getName()){
                shapesArray[i]->setRotate(angle);
                break;
            }
        }
    }
    cout<<"Rotated "<<name<<" "<<"by "<<angle<<" degrees"<<endl;
}



void moveErrCheck(string subMove, stringstream& lineStream){
    stringstream moveArg(subMove);
    stringstream testString(subMove); //create a test string
    string name;
    int xlocation,ylocation;
    int numOfArg=0;
    string testName;
    testString>>testName; //assume testName is read in successfully
                          //both testString and lineStream are the same if fail both fails
    string arguments;
    //count the number of argument in the move command
    while(moveArg>>arguments){
        numOfArg++;
    }
    //cout<<numOfArg<<endl;
    //when argument is >=1, name exists
    if(numOfArg>=1){
        lineStream>>name;
    }
        //name must be read in before the check
    if(!name.empty()){
    //check if shape name can be found or not
    //check if shape name exists
        bool nameExist= false;
        for (int i = 0; i < max_shapes; i++){
            if(shapesArray[i]!=nullptr){ //as long a pointer points to an object
                if(name==(shapesArray[i])->getName()){ //extract its name and compare
                    nameExist=true;//name exists
                    break;
                             //found name exists,break the loop
                }  
             //if no shape object exists, the for loop will be skipped
            }
        }
        if(!nameExist){
            cout<<"Error: shape "<<name<<" not found"<<endl;//error found
            return; //exit the function
        }
    }
    //second argument xloc exists while arguments>=2
    //invalid argument or not
    //value valid or not
    char next;
    int testx;
    const char* b=" ";
    if(numOfArg>=2){
       testString>>testx;
       next=testString.peek();
       lineStream>>xlocation;
        if(lineStream.fail()||((next!=*b)&&next!=EOF)){   //if not valid argument
            cout<<"Error: invalid argument"<<endl;
            return; //error found exit the function
        }
        else{           //valid argument,check value
            if(xlocation<0){
                cout<<"Error: invalid value"<<endl;
                return; //error found exit the function
            }
        }
       
    }
    
    //third argument yloc exists while arguments>=3
    //invalid argument or not
    //value valid or not
    char nexty;
    int testy;
    if(numOfArg>=3){
       testString>>testy;
       nexty=testString.peek();
       lineStream>>ylocation;
        if(lineStream.fail()||((nexty!=*b)&&nexty!=EOF)){   //if not valid argument
            cout<<"Error: invalid argument"<<endl;
            return; //error found exit the function
        }
        else{           //valid argument,check value
            if(ylocation<0){
                cout<<"Error: invalid value"<<endl;
                return; //error found exit the function
            }
        }
       
    }
    
    //too many arguments
    if(numOfArg>3){
        cout<<"Error: too many arguments"<<endl;
        return;
     }
    
    //too few arguments
    if(numOfArg<3){
        cout<<"Error: too few arguments"<<endl;
        return;
     }
    
    //if no error occurs
    for(int i=0; i<max_shapes;i++){
        if(shapesArray[i]!=nullptr){
            if(name==shapesArray[i]->getName()){
                shapesArray[i]->setXlocation(xlocation);
                shapesArray[i]->setYlocation(ylocation);
                break;
            }
        }
    }
    cout<<"Moved "<<name<<" to "<<xlocation<<" "<<ylocation<<endl;
    
    
}
void createErrCheck(string test, stringstream& newLine,stringstream& lineStream){
    int xlocation, ylocation, xsize, ysize;
    string name;
    string type;
    int numOfArg=0;
    string arguments;
    stringstream testString (test);
    string testname,testtype;
          //assume the second string read in name type successfully
    testString>>testname;
    testString>>testtype; 
    
    
    
    while(newLine>>arguments)
        numOfArg++;  //count the number of arguments 
        
    //check for error
    //read in name,type,xlocation,ylocation,xsize,ysize in order
    //priority list from left to right first one: name
    
    if(numOfArg>=1){
        lineStream>>name;
    }
        //name must be read in before the check
    if(!name.empty()){
    //check for next error invalid shape name(reserved word-command name)
        for(int i = 0;i<NUM_KEYWORDS;i++){  //iterate through the keywordlist
            if(name==keyWordsList[i]){       //compare name to its element, if equal
                cout<<"Error: invalid shape name"<<endl;  //print error message
                return; //exit the function
            }
        }
    //check for next error invalid shape name(reserved word-shape type) 
        for(int i = 0;i<NUM_TYPES;i++){  //iterate through the shapeTypeList
            if(name==shapeTypesList[i]){       //compare name to its element, if equal
                cout<<"Error: invalid shape name"<<endl;  //print error message
                return; //exit the function
            }
        }
        
    
    //check if shape name exists
        for (int i = 0; i < max_shapes; i++){
            if(shapesArray[i]!=nullptr){
                if(name==(shapesArray[i])->getName()){
                    cout<<"Error: shape "<<name<<" exists"<<endl;
                    return; //exit the function
                }
            }
        }
    }
    
            
    if(numOfArg>=2){
        lineStream>>type;
    }
    
    //type must be read in before checking
    if(!type.empty()){
        //check if the shape type is valid
        bool validShapeType= false;
        for(int i=0; i<NUM_TYPES; i++){
            if(type==shapeTypesList[i]){
                //if type == one element in the list
                //then the shape type is valid
                validShapeType=true; 
                break;
            }    
        }
    
        if(!validShapeType){
            cout<<"Error: invalid shape type"<<endl;
            return;
        }
    }
      
    //xlocation exists while arg>=3
    //read in xlocation
    //check if it is valid argument or not, priority goes before valid value
    //check if it is valid value or not 
    char next1;
    int testxloc;
    const char* c=" ";
    if(numOfArg>=3){
        testString>>testxloc;
        next1=testString.peek();
        lineStream>>xlocation;
        if(lineStream.fail()||(next1!=*c&&next1!=EOF)){   //if not valid argument
            cout<<"Error: invalid argument"<<endl;
            return; //error found exit the function
        }
        else{           //valid argument,check value
            if(xlocation<0){
                cout<<"Error: invalid value"<<endl;
                return; //error found exit the function
            }
        }
    }
    
    //ylocation exists while arg>=4
    //read in ylocation
    //check if it is valid argument or not, priority goes before valid value
    //check if it is valid value or not 
    char next2;
    int testyloc;
    if(numOfArg>=4){
        testString>>testyloc;
        next2=testString.peek();
        lineStream>>ylocation;
        if(lineStream.fail()||(next2!=*c&&next2!=EOF)){
            cout<<"Error: invalid argument"<<endl;
            return;
        }
        else{
             if(ylocation<0){
                cout<<"Error: invalid value"<<endl;
                return; //error found exit the function
             }
        }
    }
    
    //xsize exists while arg>=5
    //read in xsize
    //check if it is valid argument or not, priority goes before valid value
    //check if it is valid value or not 
    char next3;
    int testxsz;
    if(numOfArg>=5){
        testString>>testxsz;
        next3=testString.peek();
        lineStream>>xsize;
        if(lineStream.fail()||(next3!=*c && next3!=EOF)){
            cout<<"Error: invalid argument"<<endl;
            return;
        }
        else{
             if(xsize<0){
                cout<<"Error: invalid value"<<endl;
                return; //error found exit the function
             }
        }
    }
    
    //ysize exists while arg>=6
    //read in ysize
    //check if it is valid argument or not, priority goes before valid value
    //check if it is valid value or not 
    char next;
    int testysize;
    const char* b=" ";
    if(numOfArg>=6){
        testString>>testysize;
        next=testString.peek();
        lineStream>>ysize;
        if(lineStream.fail()||(next!=*b&&next!=EOF)){
            cout<<"Error: invalid argument"<<endl;
            return;
        }
        else{
             if(ysize<0){
                cout<<"Error: invalid value"<<endl;
                return; //error found exit the function
             }
             
             if(type=="circle"){
                 if(ysize!=xsize){//check size for circle
                    cout<<"Error: invalid value"<<endl;
                    return; //error found exit the function   
                 }
                     
             }
        }
    }
    
    //many argument goes before few arguments
    //when argument is greater than 6
    if(numOfArg>6){
        cout<<"Error: too many arguments"<<endl;
        return;
     }
    
    
    
     //when argument is less than six 
     if(numOfArg<6){
        cout<<"Error: too few arguments"<<endl;
        return;
     }
    
    //when the shape array is full
    //when some shape pointers are deleted, that pointer will be null 
    //thought the location cant be used anymore
    //count the array spot being allocated shapeCount 
    //if shapeCount==max_shapes
    //array is full
    if(shapeCount==max_shapes){
        cout<<"Error: shape array is full"<<endl;
        return;
    }
    
    createShape(name,type,xlocation,ylocation,xsize,ysize);
    //no error, create shape object 
    cout<<"Created "<<name<<": "<<type<<" "<<xlocation<<" "<<ylocation<<" "<<xsize<<" "<<ysize<<endl;
}

void createShape(string n,string t,int x_loc,int y_loc,int x_sz, int y_sz){
    shapesArray[shapeCount]= new Shape(n,t,x_loc,x_sz,y_loc,y_sz);
    shapeCount++;
}