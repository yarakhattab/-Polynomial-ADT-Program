//yara khattab 1210520
#include <stdio.h>
#include <stdlib.h>


// stack implemantation
typedef struct node* mynode;

struct node {
    int data;
    mynode next;//pointer to the next node
};

typedef struct node* mystack;
int sizee = 0;//size of the stack

// linked list structure implemantation
struct myterm {
    int termfactor;  //the factore number of a term
    int termpower;    // the power of a term
    struct myterm* next; //pointer to the next node
};

typedef struct myterm* mypolynomial;


// the functions
mystack createStack();  // Create a stack
void push(mystack Top, int num);  // push elements to the stack
void pop(mystack Top);  // pop elemants from stack
int isEmpty(mystack Top);  // Check if the stack empty
void makeEmpty(mystack Top);  // make the stack empty
int top(mystack Top);  // Return the top element of the stack
void printList(mystack S); //function to print the elemants in the stack

mypolynomial combinetheterms(mypolynomial poly);// function to combine the equal terms ==> using in multiplication
mypolynomial parseequation(char* line);// function to parse the equations to allow to store in a linked list
void printmmypolynomial(mypolynomial poly); //function to print the equations from the linked list
void sortmypolynomial(mypolynomial* poly); // function to sort the equations from higher to lower power
mypolynomial addmypolynomials(mypolynomial poly1, mypolynomial poly2); //addition operation
mypolynomial subtractmypolynomials(mypolynomial poly1, mypolynomial poly2);// subtraction
mypolynomial multiplymypolynomials(mypolynomial poly1, mypolynomial poly2);//multiplication
void writetoouputfile(mypolynomial addres, mypolynomial subres, mypolynomial mulres, const char *filename,int x,int y,int z);// function to write the results to output file


// function to create a new stack it is return a stack
mystack createStack() {
    mystack s;
    s = (mystack)malloc(sizeof(struct node));//allocate memory
    if (s == NULL) {
        printf("can not create stack.\n");
        return NULL;
    } else {
        s->next = NULL;
    }
    return s;
}

// Push to the stack (it push a polynomial)
void push(mystack Top, int num) {
    mynode newNode = (mynode)malloc(sizeof(struct node));//allocate memory
    newNode->data = num;

    mynode temp;

    if (Top == NULL) {
        printf("can not found the stack\n");
        return;
    } else if (newNode == NULL) {
        printf("Out of space\n");
        return;
    } else if (isEmpty(Top)) { //check if the stack is emmpty
        newNode->next = Top->next;
        Top->next = newNode;
        sizee++;
        return;
    }

    temp = Top->next;
    Top->next = newNode;
    newNode->next = temp;
    sizee++;
}

// Pop the top element of the stack
void pop(mystack Top) {
    mynode temp;

    if (Top == NULL) {
        printf("can not found the stack\n");
        return;
    } else if (isEmpty(Top)) {
        printf("Stack Underflow\n");
        return;
    } else if (Top->next->next == NULL) {
        temp = Top->next;
        Top->next = NULL;
        free(temp);
        sizee--;
        return;
    }

    mynode ptr = Top->next->next;
    temp = Top->next;
    Top->next = ptr;
    free(temp);
    sizee--;
}

// Check if the stack is empty
int isEmpty(mystack Top) {
    return (Top->next == NULL);
}

// make the stack empty
void makeEmpty(mystack Top) {
    if (Top == NULL) {
        printf("can not found the stack\n");
        return;
    } else if (isEmpty(Top)) {
        printf("Stack Underflow\n");
        return;
    }

    while (!isEmpty(Top)) {
        pop(Top); // start to make it empty by pop it's elements
    }

    printf("stack has been deleted\n");
    return;
}

// Return the top element of the stack
int top(mystack Top) {
    if (Top == NULL) {
        printf("can not found the stack\n");
        return -1;
    } else if (isEmpty(Top)) {
        printf("Stack is empty,there is nothing to return \n");
        return -1;
    }
    return Top->next->data; // return the top of the stack if it exist
}


// function to  parse the Polynomial
mypolynomial parseequation(char* line) {
    mypolynomial newlist = NULL, last = NULL; // a pointers for the linked list for the head and last of the list
    int i = 0, coffiecent = 0, pow = 0, termsign = 1, readfactore = 1;

    while (line[i] != '\0' && line[i] != '\n') { //to check if we reach the end of line
        // Check for sign of the term
        if (line[i] == '-') {
            termsign = -1;
            i++;
        } else if (line[i] == '+') {
            termsign = 1;
            i++;
        }

        coffiecent = 0;  // reset coefficent
        readfactore = 1;

        // Read coefficent or set to 1 if the term is only 'x'
        while (line[i] >= '0' && line[i] <= '9') { //check if the coffiecent is a number between 0-9
            coffiecent = coffiecent * 10 + (line[i] - '0');
            i++;
            readfactore = 0;
        }

        if (readfactore)  // If no number assume it is 1
            coffiecent = 1;
        coffiecent *= termsign; // multiply the coffiecent with the sign of the term

        // Check for variable'x' and parse the power
        pow = 0;
        if (line[i] == 'x') {
            pow = 1;  // assume the power is 1
            i++;

            if (line[i] == '^') { // if the power greater or equal 2
                i++;
                pow = 0;
                while (line[i] >= '0' && line[i] <= '9') { // check if the power is a number
                    pow = pow * 10 + (line[i] - '0');
                    i++;
                }
            }
        }

        // Create a new term (node) to store the coffiecent and power
        mypolynomial newTerm = (mypolynomial)malloc(sizeof(struct myterm));// allocate memory
        newTerm->termfactor = coffiecent; //set the factor in the node
        newTerm->termpower = pow; //set the power in the node
        newTerm->next = NULL;

        // insert the node (have the term) in the linked list
        if (newlist == NULL) { // this is for the first node to insert
            newlist = newTerm;
        } else {
            last->next = newTerm; // if the linked list is not empty then insert to the end of list
        }
        last = newTerm; // make the pointer last to point the last node in the list (update it)
    }
    return newlist;
}

// print a polynomial
void printmmypolynomial(mypolynomial p) {
    mypolynomial current = p; // pointer to point the list
    while (current != NULL) {
        if (current->termfactor > 0 && current != p) {
            printf("+");
        }
        printf("%dx^%d ", current->termfactor, current->termpower);
        current = current->next;
    }
    printf("\n");//print mew line
}

// sort the polynomial terms from heigher to lower power
void sortmypolynomial(mypolynomial* poly) {
    if (*poly == NULL || (*poly)->next == NULL) {
        return;  // return as it if the list have 0 or 1 element
    }

    mypolynomial current, prev = NULL, next = NULL;
    int swap; //to swap the terms

    do {
        swap = 0;
        current = *poly; //pointer to the list

        while (current != NULL && current->next != NULL) {
            next = current->next;
            if (current->termpower < next->termpower) {
                // swap the terms
                if (prev != NULL) {
                    prev->next = next;
                } else {
                    *poly = next;
                }

                current->next = next->next;
                next->next = current;
                swap = 1;
                prev = next;
            } else {
                prev = current;
                current = current->next;
            }
        }
    } while (swap);
}

// function to print the elements in the stack
void printList(mystack S) {
    if (S == NULL) {
        printf("can not found the stack\n");
        return;
    } else if (isEmpty(S)) {
        printf("Stack is empty, nothing to print\n");
        return;
    }


    mynode temp = S->next;  // Starting from the first element in the stack

    while (temp != NULL) {
        mypolynomial mypoly = temp->data;  // get polynomial stored in the stack node

        // Print the polynomial
        printf("The Polynomial: ");
        printmmypolynomial(mypoly);  // Call the function that prints individual polynomials
        temp = temp->next;
    }
    printf("\n");
}



// add function
mypolynomial addmypolynomials(mypolynomial firstpoly, mypolynomial secondpoly) {
    mypolynomial theresult = NULL, last = NULL;

// check if the polynomials not Null
while (firstpoly != NULL || secondpoly != NULL) {
    int firstcoeff = 0, secondcoeff = 0, firstexp = 0, secondexp = 0;

    // get the values from firstpoly if it is not NULL
    if (firstpoly != NULL) {
        firstcoeff = firstpoly->termfactor;
        firstexp = firstpoly->termpower;
    }

    // get the values from secondpoly if it is not NULL
    if (secondpoly != NULL) {
        secondcoeff = secondpoly->termfactor;
        secondexp = secondpoly->termpower;
    }

    // create a new node term to add to the result
    mypolynomial newTerm = (mypolynomial)malloc(sizeof(struct myterm));//alocate memory

    // check if the exponents are equal
    if (firstexp == secondexp) {
        newTerm->termfactor = firstcoeff + secondcoeff;  // add the coefficients of the equal terms
        newTerm->termpower = firstexp;

        // move to the next node term in firstpoly if not NULL
        if (firstpoly != NULL) {
            firstpoly = firstpoly->next;
        } else {
            firstpoly = NULL;
        }

        // move to the next node term in secondpoly if not NULL
        if (secondpoly != NULL) {
            secondpoly = secondpoly->next;
        } else {
            secondpoly = NULL;
        }
    }

    // If firstexp > secondexp then take term from firstpoly
    if (firstexp > secondexp) {
        newTerm->termfactor = firstcoeff;
        newTerm->termpower = firstexp;

        // move to the next node term in firstpoly if not NULL
        if (firstpoly != NULL) {
            firstpoly = firstpoly->next;
        } else {
            firstpoly = NULL;
        }
    }

    // If secondexp > firstexp, take term from secondpoly
    if (secondexp > firstexp) {
        newTerm->termfactor = secondcoeff;
        newTerm->termpower = secondexp;

        // move to the next node term in secondpoly if not NULL
        if (secondpoly != NULL) {
            secondpoly = secondpoly->next;
        } else {
            secondpoly = NULL;
        }
    }

    // If the coefficient is not zero, add the term to the result list
    if (newTerm->termfactor != 0) {
        newTerm->next = NULL;
        if (theresult == NULL) {
            theresult = newTerm;  // First term in the result list
        } else {
            last->next = newTerm;  // append the term to the result list
        }
        last = newTerm;  // update the last pointer to point to the last node in the list
    } else {
        free(newTerm);  // Free memory if the coefficient is 0
    }
}
    return theresult;//return the list of add result
}


//subtraction
mypolynomial subtractmypolynomials(mypolynomial firstpoly, mypolynomial secondpoly) {
    mypolynomial result = NULL, last = NULL;

   // check if the two poly exist
while (firstpoly != NULL || secondpoly != NULL) {
    int firstcoeff, secondcoeff, firstexp, secondexp;

    // get the values from firstpoly if it is not NULL
    if (firstpoly != NULL) {
        firstcoeff = firstpoly->termfactor;
        firstexp = firstpoly->termpower;
    } else {
        firstcoeff = 0;
        firstexp = 0;
    }

    // get the values from secondpoly if it is not NULL
    if (secondpoly != NULL) {
        secondcoeff = secondpoly->termfactor;
        secondexp = secondpoly->termpower;
    } else {
        secondcoeff = 0;
        secondexp = 0;
    }

        mypolynomial newTerm = (mypolynomial)malloc(sizeof(struct myterm));// create a new node term to save the result

        // check if the two powers equal
        if (firstexp == secondexp) {
         newTerm->termfactor = firstcoeff - secondcoeff;  // Subtract coefficients for equal terms
         newTerm->termpower = firstexp;

    // move to the next node term in first polynomial
    if (firstpoly != NULL) {
        firstpoly = firstpoly->next;
    } else {
        firstpoly = NULL;
    }

     // move to the next node term in second polynomial
    if (secondpoly != NULL) {
        secondpoly = secondpoly->next;
    } else {
        secondpoly = NULL;
    }


}

// check if the power of the first term greater then the power of second term
 else if (firstexp > secondexp) {
    newTerm->termfactor = firstcoeff;  // get firstcoeff if firstexp > secondexp
    newTerm->termpower = firstexp;

    // move to the next node term in first polynomial
    if (firstpoly != NULL) {
        firstpoly = firstpoly->next;
    } else {
        firstpoly = NULL;
    }


}
 // check if the power of the first term less then the power of second term
 else {
    newTerm->termfactor = -secondcoeff;  // get and negate secondcoeff for subtraction
    newTerm->termpower = secondexp;

    // move to the next node term in second polynomial
    if (secondpoly != NULL) {
        secondpoly = secondpoly->next;
    } else {
        secondpoly = NULL;
    }
}

        // If the coefficient is not 0, add the term to the result list
        if (newTerm->termfactor != 0) {
            newTerm->next = NULL;
            if (result == NULL) {
                result = newTerm;  // First term in the result list
            } else {
                last->next = newTerm; // append the term to the result list
            }
            last = newTerm;    // update the last pointer to point to the last node in the list
        } else {
            free(newTerm);  // Free if the coefficient is 0
        }
    }

    return result; //return the list of sub result
}

mypolynomial multiplymypolynomials(mypolynomial firstpoly, mypolynomial secondpoly) {
    mypolynomial result = NULL, last = NULL;


    while (firstpoly != NULL) {
        mypolynomial currentPoly2 = secondpoly;

        while (currentPoly2 != NULL) {
            // Multiply the coefficients and add the exponents
            int coeff = firstpoly->termfactor * currentPoly2->termfactor;
            int exp = firstpoly->termpower + currentPoly2->termpower;

            // Create the new term
            mypolynomial newTerm = (mypolynomial)malloc(sizeof(struct myterm));
            newTerm->termfactor = coeff;
            newTerm->termpower = exp;
            newTerm->next = NULL;

            // Insert the new term into the result list
            if (result == NULL) {
                result = newTerm;
            } else {
                last->next = newTerm;
            }
            last = newTerm;
            currentPoly2 = currentPoly2->next;  // move to the next term in secondpoly
        }
        firstpoly = firstpoly->next;  // move to the next term in firstpoly
    }

    // Combine equal terms
    result = combinetheterms(result);

    return result;
}

// combine terms function
mypolynomial combinetheterms(mypolynomial result) {
    mypolynomial tempresult = result;

    while (tempresult != NULL && tempresult->next != NULL) {
        // Check for same exponent
        mypolynomial prev = tempresult;
        mypolynomial next = tempresult->next;
        while (next != NULL) {
            if (tempresult->termpower == next->termpower) {
                // Combine the terms by adding their coefficients
                tempresult->termfactor += next->termfactor;

                // next term
                prev->next = next->next;
                free(next);
                next = prev->next;  // move to the next term
            } else {
                prev = next;
                next = next->next;
            }
        }
        tempresult = tempresult->next;
    }

    return result;
}

// function to write the results to the file
//addres,,subres,mulres are linked lists for the result of the opertion
// integers x,y,z are flages to check if the user choose which operation
void writetoouputfile(mypolynomial addres, mypolynomial subres, mypolynomial mulres, const char *filename,int x,int y,int z) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Unable to open file %s for writing.\n", filename);
        return;
    }

    fprintf(file, "Results of Operations are :\n");

    //addition result
    fprintf(file, "\nAddition Result:\n");

    if (x>0) { //check if user chosse the addition
        mypolynomial current = addres;//linked list that have the result of add
        while (current != NULL) {
            fprintf(file, "%dx^%d", current->termfactor, current->termpower);
            if (current->next != NULL) {
                fprintf(file, " + ");
            }
            current = current->next; //move to the next term
        }
        fprintf(file, "\n");//new line

    }

    else {
        fprintf(file, "No addition result available.\n");// if the user did not chosse addition
    }

    // subtraction result
    fprintf(file, "\nSubtraction Result:\n");
    if (y>0) { //check if user chosse the subtraction
        mypolynomial current = subres;//linked list that have the result of sub
        while (current != NULL) {
            fprintf(file, "%dx^%d", current->termfactor, current->termpower);
            if (current->next != NULL) {
                fprintf(file, " + ");
            }
            current = current->next;// move to the next term
        }
        fprintf(file, "\n");

    }

    else {
        fprintf(file, "No subtraction result available.\n");// if the user did not chosse subtraction
    }

    // multiplication result
    fprintf(file, "\nMultiplication Result:\n");
    if (z>0) { //check if user chosse the multyplication
        mypolynomial current = mulres; //linked list that have the result of multiply
        while (current != NULL) {
            fprintf(file, "%dx^%d", current->termfactor, current->termpower);
            if (current->next != NULL) {
                fprintf(file, " + ");
            }
            current = current->next;
        }
        fprintf(file, "\n");
    } else {
        fprintf(file, "No multiplication result available.\n");// if the user did not chosse multiplucation
    }

    fclose(file);
    printf("Results written to file %s successfully.\n", filename);
}



// main function
int main() {
    mystack polyStack = createStack();  // stack of polynomials
    mypolynomial subres;// sub result list
    mypolynomial addres;// add result list
    mypolynomial mulres; // mul result list
    int choice;

    int addflag = 0; //flag to track if user choose add
    int subflag = 0;  //flag to track if user choose sub
    int mulflag = 0;  //flag to track if user choose mul


    do {
        // print the menu
        printf("\n********* Welcome to Polynomial ADT Program ***********\n");
        printf("please choose the operation you want from 1 to 8\n");
        printf("1. Load the polynomials from file\n");
        printf("2. Print the polynomials\n");
        printf("3. make addition\n");
        printf("4. make subtraction \n");
        printf("5. make mutiplication \n");
        printf("6. Print the results to	the screen\n");
        printf("7. Print the results to	the file\n");
        printf("8. exit from the program\n\n ");
        printf("Please Enter your choice:");
        scanf("%d", &choice);

         // switch statment to go throw the menu operations
        switch (choice) {
            case 1: {
                //read from the input file and check if it exist
                FILE* file = fopen("input.txt", "r");
                if (file == NULL) {
                    printf("can not open file.\n");
                    break;
                }

                char line[4096];
                while (fgets(line, sizeof(line), file)) { //get line by line from the file
                    mypolynomial poly = parseequation(line);// parse the line (line contains the polynomial)

                    sortmypolynomial(&poly);   // sort the polynomial

                    push(polyStack,poly); // push the sorted polynomial to the stack of polynomials

                }

                printf("polynomials loaded successfully\n ");
                fclose(file);

                break;
            }

            // print the polynomials
            case 2: {
                mystack tempStack = createStack();  // temporary stack to hold the popped elements from the original stack of polynomials
                mynode current = polyStack->next;   // pointer to point the original stack

                // pop elements from the original stack into the temporary stack
                while (current != NULL) {
                push(tempStack, current->data);  // Push into the temporary stack
                current = current->next;
            }
             printList(tempStack); // print the stack of polynomials
                break;
            }

             // addition
            case 3: {

            if (isEmpty(polyStack)) {
            printf("No polynomials to add.\n");
            break;
            }

            mystack addStack = createStack();  // temporary stack for the addition operation
            mypolynomial res = NULL; // a list for the result of addition

    //Pop all polynomials from the original stack and push to the addStack
    while (!isEmpty(polyStack)) {
        mypolynomial poly =top(polyStack);
        pop(polyStack);  // pop elements from the original stack

        push(addStack,poly);  // push the elements into the addStack
    }

    // restore the original stack by pushing elements back from the temporary stack to use it for other operation
    mynode current = addStack->next;

    current = addStack->next;
    while (current != NULL) {
        push(polyStack, current->data);
        current = current->next;
    }



    //  add all polynomials in addStack
    while (!isEmpty(addStack)) {
        mypolynomial poly = (mypolynomial)top(addStack);
        pop(addStack);  // pop from addStack

        // If sum is NULL, initialize it with the first polynomial
        if (res == NULL) {
            res = poly;
        }
        else {
            res = addmypolynomials(res, poly);  // add to the sum
        }

    }

    addres = res;// save the result of add
    addflag++;//incrimant the flag
    printf("the add operation success\n");


    break;
}

//subtraction
case 4: {

    if (isEmpty(polyStack)) {
        printf("No polynomials to subtract.\n");
        break;
    }

    mystack subStack = createStack();  // temporary stack for the subtraction operation
    mypolynomial res = NULL;

    // Pop all polynomials from the original stack and push to the subStack
    while (!isEmpty(polyStack)) {
        mypolynomial poly = top(polyStack);
        pop(polyStack);  // pop from the original stack

        push(subStack, poly);  // Push in the subStack
    }

    // restore the original stack by pushing elements back from the temporary stack
    mynode current = subStack->next;

    current = subStack->next;
    while (current != NULL) {
        push(polyStack, current->data);
        current = current->next;
    }


    //Subtract all polynomials in subStack
    if (!isEmpty(subStack)) {
        res = (mypolynomial)top(subStack);
        pop(subStack);  // Start with the first polynomial in the stack

        while (!isEmpty(subStack)) {
            mypolynomial poly = (mypolynomial)top(subStack);
            pop(subStack);  // Remove from subStack

            res = subtractmypolynomials(res, poly);  // Subtract the polynomial from the result
        }
    }

    subres = res;//save the result of subtraction
    subflag++;//incrimant the flag of sub
     printf("the sub operation success\n");

    break;
}

//multiplication
case 5: {
    if (isEmpty(polyStack)) {
        printf("No polynomials to multiply.\n");
        break;
    }

    mystack mulStack = createStack();  // temporary stack for the multiplication operation
    mypolynomial res = NULL;

    //Pop all polynomials from the original stack and push to the mulStack
    while (!isEmpty(polyStack)) {
        mypolynomial poly = (mypolynomial)top(polyStack);
        pop(polyStack);  // pop from the original stack

        push(mulStack, poly);  // Push in the mulStack
    }

    // restore the original stack by pushing elements back from the temporary stack
    mynode current = mulStack->next;

    current = mulStack->next;
    while (current != NULL) {
        push(polyStack, current->data);
        current = current->next;
    }

    //Multiply all polynomials in mulStack
    if (!isEmpty(mulStack)) {
        res = (mypolynomial)top(mulStack);
        pop(mulStack);  // Start with the first polynomial in the stack

        while (!isEmpty(mulStack)) {
            mypolynomial poly = (mypolynomial)top(mulStack);
            pop(mulStack);  // Remove from mulStack

            res = multiplymypolynomials(res, poly);  // Multiply the polynomials
        }
    }

    mulres =res;//save the result of multiplication
    mulflag++;//incrimant the mul flag
    printf("the multiplication operation success\n");
    break;
}

        // print the results of operations
        case 6: {


                if (addflag >0) { // check if the addflsg >0 so there is a result for it so print it
                    printf("The addition result of polynomials:");
                    printmmypolynomial(addres);  // Print addition result
                } else {
                    printf("You did not choose add operation.\n");
                }

                if (subflag >0) { // check if the subflsg >0 so there is a result for it so print it
                    printf("The subtraction result of polynomials:");
                    printmmypolynomial(subres);  // Print subtraction result
                } else {
                    printf("You did not choose sub operation.\n");
                }

                if (mulflag >0) { // check if the mulflsg >0 so there is a result for it so print it
                    printf("The multiplication result of polynomials:");
                    printmmypolynomial(mulres);  // Print multiplication result
                } else {
                    printf("You did not choose multiplication operation.\n");
                }
                break;
            }

          // write the results to the file
        case 7:
            {
            writetoouputfile(addres,subres,mulres,"result.txt",addflag,subflag,mulflag);

            break;
            }

            // exiting
        case 8 :
            {
                printf("exit from the program");
            break;

            }
             // if the user enter a number >8
            default: {
                printf("Invalid choice! Please enter a valid option.\n");
                break;
            }
        }
    } while (choice != 8);


    return 0;
}
