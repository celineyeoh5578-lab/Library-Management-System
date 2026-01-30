#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
using namespace std;

struct Book
{
    string id;
    string title;
    string author;
    string category;
    int quantity;
};

Book newBook(string m_id,string t,string au,string c,int qty){
    Book book;
    book.id=m_id;
    book.title=t;
    book.author=au;
    book.category=c;
    book.quantity=qty;

    return book;
}

vector<Book> books;

bool validInt(int& input){
    if(cin.fail()){
        cin.clear();
        cin.ignore(1000,'\n');
        return false;
    }
    return true;
}

string trim(const string& str){
    size_t start=str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == string::npos) ? "" : str.substr(start, end - start + 1);
}

vector<vector<string>> readCSV(string filename){
    vector<vector<string>> data;
    string line;

    ifstream file(filename);
    if(!file.is_open()){
        cerr << "Error: Could not open file " << filename << endl;
        return data;
    }

    while(getline(file,line)){
        vector<string> row;
        stringstream ss(line);
        string value;

        while(getline(ss,value,',')){
            row.push_back(value);
        }
        data.push_back(row);
    }
    file.close();
    return data;
}

bool authenticateAdmin(string username,string password)
{
    if(username=="admin"&&password=="123456"){
        cout<<"Authentication successful!"<<endl;
        return true;
    }
    else{
        cout<<"Invalid credentials!"<<endl;
        return false;
    }
}

void loadBooksFromCSV(string filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Failed to open the file for reading!" << endl;
        return;
    }

    books.clear();
    string line;
    getline(file, line); // Skip header row

    while (getline(file, line))
    {
        stringstream ss(line);
        string id, title, author, category,quantityStr;
        int quantity;

        getline(ss, id, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, category, ',');
        getline(ss, quantityStr, ',');

        quantity=stoi(quantityStr);
        books.push_back(newBook(id, title, author, category, quantity));
    }
    file.close();
}

void saveBooksToCSV(string filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << "Failed to open the file for writing!" << endl;
        return;
    }

    file << "ID,Title,Author,Category,Available" << endl;
    for (int i=0; i<books.size(); i++)
    {
        file << books[i].id << "," << books[i].title << "," << books[i].author << "," << books[i].category << "," << books[i].quantity << endl;
    }
    file.close();
}

void addBook(string m_id, string t, string au, string c, int av, string filename){

    loadBooksFromCSV(filename); // Load current books

    for (int i=0; i<books.size(); i++)
    {
        if (books[i].id == m_id)
        {
            cout << "Book already exists!" << endl;
            return;
        }
    }

    struct Book book=newBook(m_id, t, au, c, av);
    books.push_back(book);
    saveBooksToCSV(filename); // Save updated books
    cout << "Book added successfully!" << endl;
}

void deleteBook(string m_id, string filename)
{
    loadBooksFromCSV(filename); // Load current books

    int indexToDelete= -1;

    for (int i=0; i<books.size(); i++)
    {
        if (books[i].id == m_id)
        {
           indexToDelete=i;
            break;
        }
    }

    if(indexToDelete !=-1)
    {
        books.erase(books.begin()+indexToDelete);
        saveBooksToCSV(filename); // Save updated books
        cout << "Book deleted successfully!" << endl;
    }

    else
        cout << "Book not found!" << endl;


}



void updateQuantity(string bookID,int qty,string filename){
    loadBooksFromCSV(filename);
    for(int i=0;i<books.size();i++){
        if(books[i].id==bookID){
            books[i].quantity-=qty;
            saveBooksToCSV(filename);
            return;
        }
    }
}

void editBookDetails(string m_id, string t, string au, string c, int av, string filename){
    loadBooksFromCSV(filename); // Load current books

    for (int i=0; i<books.size(); i++)
    {
        if (books[i].id == m_id)
        {
            books[i].title = t;
            books[i].author = au;
            books[i].category = c;
            books[i].quantity = av;
            saveBooksToCSV(filename); // Save updated books
            cout << "Book details updated successfully!" << endl;
            return;
        }
    }

    cout << "Book not found!" << endl;
}

void viewAllBooks(string filename){
    vector<vector<string>> bookData=readCSV(filename);
    cout<<left;
    cout<<"\nBook List"<<endl;
    cout<<setw(15)<<"Book ID"<<setw(35)<<"Book Title";
    cout<<setw(25)<<"Author Name"<<setw(20)<<"Book Category"<<setw(10)<<"Quantity"<<endl;

    for(int i=1;i<bookData.size();i++){
        cout<<setw(15)<<trim(bookData[i][0])<<setw(35)<<trim(bookData[i][1]);
        cout<<setw(25)<<trim(bookData[i][2])<<setw(20)<<trim(bookData[i][3])<<setw(10)<<trim(bookData[i][4])<<endl;
    }
}

void merge(vector<Book>& books,int left, int mid, int right)
{
    int n1=mid-left+1;
    int n2= right-mid;
    vector<Book> L(n1),R(n2);

    for(int i=0;i<n1;i++)
    {
        L[i]=books[left+i];
    }
    for(int j=0;j<n2;j++)
    {
        R[j]=books[mid+1+j];
    }
    int i=0,j=0;
    int k=left;

    while(i<n1 && j<n2)
    {
        if(L[i].id<R[j].id)
        {
            books[k]=L[i];
            i++;
        }
        else
        {
            books[k]=R[j];
            j++;
        }
        k++;
    }

    while(i<n1)
    {
        books[k]=L[i];
        i++;
        k++;
    }

    while(j<n2)
    {
        books[k]=R[j];
        j++;
        k++;
    }
}

void mergeSortByID(vector<Book>& books, int left, int right)
{
    if(left>=right)
        return;

    int mid= left+(right-left)/2;
    mergeSortByID(books,left,mid);
    mergeSortByID(books,mid+1,right);
    merge(books,left,mid,right);

}

void bubbleSortByTitle(vector<Book>& books)
{
    int n= books.size();
    for(int i=0; i<n-1; i++){
        for(int j=0; j<n-i-1; j++){
            if(books[j].title>books[j+1].title){
                swap(books[j],books[j+1]);
            }
        }
    }
}

void selectionSort(vector<vector<string>>& vec,int columnIndex){
    int size=vec.size();
    if(size==0||columnIndex<0)
        return;

    for(int i=0;i<size-1;i++){
        int minIndex=i;
        for(int j=i+1;j<size;j++){
            if(vec[j][columnIndex]<vec[minIndex][columnIndex])
                minIndex=j;
        }

        if(minIndex!=i)
            swap(vec[i],vec[minIndex]);
    }
}

void sortBooksByID(string filename)
{
    loadBooksFromCSV(filename);
    mergeSortByID(books,0,books.size()-1);

    cout << left;
    cout << "\nBooks sorted by ID:"<<endl;
    cout <<setw(15)<<"Book ID"<<setw(35)<<"Book Title"<<setw(25)<<"Author Name"<<setw(20)<<"Book Category"<<setw(10)<<"Quantity"<<endl;

    for (int i=0;i<books.size();i++)
        {
            cout <<setw(15)<<books[i].id<<setw(35)<<books[i].title<<setw(25)<<books[i].author<<setw(20)<<books[i].category<<setw(10)<<books[i].quantity<<endl;
        }
}

void sortBooksByTitle(string filename)
{
    loadBooksFromCSV(filename);
    bubbleSortByTitle(books);

    cout << left;
    cout << "\nBooks sorted by Title:\n";
    cout <<setw(15)<<"Book ID"<<setw(35)<<"Book Title"<<setw(25)<<"Author Name"<<setw(20)<<"Book Category"<<setw(10)<<"Quantity"<<endl;

    for (int i=0;i<books.size();i++) {
        cout<<setw(15)<<books[i].id<<setw(35)<<books[i].title<<setw(25)<<books[i].author<<setw(20)<<books[i].category<<setw(10)<<books[i].quantity<<endl;
    }
}

bool checkSubstring(string title,string bookname){
    return title.find(bookname)!=string::npos;
}

void printRow(vector<vector<string>> data,int index,int skip){
    cout<<"Book ID      : "<<data[index][0+skip]<<endl;
    cout<<"Book Title   : "<<data[index][1+skip]<<endl;
    cout<<"Author       : "<<data[index][2+skip]<<endl;
    cout<<"Category     : "<<data[index][3+skip]<<endl;
    cout<<"Availability : "<<data[index][4+skip]<<endl;
    cout<<endl;
}

void printRow2(const vector<vector<string>>& data, int rowIndex, int offset = 0) {
    cout<<left;
    cout<<setw(10)<<trim(data[rowIndex][0])<<setw(20)<<trim(data[rowIndex][1])<<setw(15)<<trim(data[rowIndex][2]);
    cout<<setw(15)<<trim(data[rowIndex][3])<<setw(35)<<trim(data[rowIndex][4])<<setw(25)<<trim(data[rowIndex][5]);
    cout<<setw(20)<<trim(data[rowIndex][6])<<setw(10)<<trim(data[rowIndex][7])<<endl;
}


void transactionReceipt()
{
    vector<vector<string>> transactions = readCSV("TransactionHistory.csv");
    selectionSort(transactions,1);

    cout<<"\nTransaction Receipts (Sorted by receipt number)"<<endl;
    cout<<left;
    cout<<setw(10)<<"User ID"<<setw(20)<<"Receipt Number"<<setw(15) << "Return Due";
    cout<<setw(15)<<"Book ID"<<setw(35)<<"Book Title"<<setw(25) << "Author Name";
    cout<<setw(20)<<"Book Category"<<setw(10)<< "Quantity"<<endl;


    for(int i=0;i<transactions.size();i++){
        cout<<left;
        cout<<setw(10)<<trim(transactions[i][0])<<setw(20)<<trim(transactions[i][1])<<setw(15)<<trim(transactions[i][2]);
        cout<<setw(15)<<trim(transactions[i][3])<<setw(35)<<trim(transactions[i][4])<<setw(25)<<trim(transactions[i][5]);
        cout<<setw(20)<<trim(transactions[i][6])<<setw(10)<<trim(transactions[i][7])<<endl;
    }
}

void sortByColumn(vector<vector<string>>& data, int columnIndex) {
    sort(data.begin(), data.end(),
         [columnIndex](const vector<string>& a, const vector<string>& b) {
             return a[columnIndex] < b[columnIndex];
         });
}

void binarySearchByUserID(vector<vector<string>> data, int columnIndex, string& userID)
 {
    int left = 0;
    int right = data.size() - 1;
    vector<int> userIndex;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (checkSubstring(data[mid][columnIndex], userID)) {
            int temp = mid;

            while (temp >= left && checkSubstring(data[temp][columnIndex], userID)) {
                userIndex.push_back(temp);
                temp--;
            }

            temp = mid + 1;
            while (temp <= right && checkSubstring(data[temp][columnIndex], userID)) {
                userIndex.push_back(temp);
                temp++;
            }
            break;
        }

        if (data[mid][columnIndex] < userID)
            left = mid + 1;
        else
            right = mid - 1;
    }

    if (userIndex.empty()) {
        cout << "No transactions found for User ID " << userID << endl;
    } else {
        cout << "\n" << userIndex.size() << " transaction(s) found for User ID " << userID << endl;
        for (int i = 0; i < userIndex.size(); i++) {
            printRow2(data, userIndex[i], 0);
        }
    }
}

void searchTransactionByUserID(string filename, string target) {
    vector<vector<string>> transactionData = readCSV(filename);
    if (transactionData.empty()) {
        cout << "Failed to load transaction history or file is empty." << endl;
        return;
    }

    int columnIndex = 0; // Assuming User ID is in the first column
    sortByColumn(transactionData, columnIndex);
    binarySearchByUserID(transactionData, columnIndex, target);
}

string getReturnDate() {
    time_t now = time(0);
    tm* localTime = localtime(&now);

    localTime->tm_mday += 15;

    mktime(localTime);

    stringstream dateStream;
    dateStream << 1900 + localTime->tm_year << "-"
               << setw(2) << setfill('0') << 1 + localTime->tm_mon << "-"
               << setw(2) << setfill('0') << localTime->tm_mday;

    return dateStream.str();
}

void binarySearchByBookTitle(vector<vector<string>> data,int columnIndex,string& bookname){
    int left=0;
    int right=data.size()-1;
    vector<int> bookIndex;

    while(left<=right){
        int mid=left+(right-left)/2;

        if(checkSubstring(data[mid][columnIndex],bookname)){
            int temp=mid;

            // Search left
            while(temp>=left && checkSubstring(data[temp][columnIndex],bookname)){
                bookIndex.push_back(temp);
                temp--;
            }

            // Search right
            temp=mid+1;
            while (temp <= right && checkSubstring(data[temp][columnIndex],bookname)) {
                bookIndex.push_back(temp);
                temp++;
            }
            break;
        }

        if(data[mid][columnIndex]<bookname)
            left=mid+1;
        else
            right=mid-1;
    }

    // Print the book
    if(bookIndex.empty()){
       cout<<"No book found."<<endl;
    }
    else{
        cout<<"\n"<<bookIndex.size()<<" book(s) found:\n";
        for(int i=0;i<bookIndex.size();i++){
            printRow(data,bookIndex[i],0);
        }
    }
}

void searchBookByTitle(string filename,string& target){
    vector<vector<string>> bookData=readCSV(filename);
    int columnIndex=1;
    sortByColumn(bookData,columnIndex);
    binarySearchByBookTitle(bookData,columnIndex,target);

}

int randomReceiptNumber(){
    srand(time(0));
    int random = 1000000+rand()%9000000;
    return random;
}

void TransactionHistory(string key){
    int columnIndex=0;
    vector<vector<string>> history=readCSV("TransactionHistory.csv");
    vector<vector<string>> userHistory;

    for(int i=0;i<history.size();i++){
        if(history[i][columnIndex]==key)
            userHistory.push_back(history[i]);
    }
    cout<<left;
    cout<<"\nTransaction History of User "<<key<<endl;
    cout<<setw(20)<<"Receipt Number"<<setw(15)<<"Book ID"<<setw(35)<<"Book Title";
    cout<<setw(25)<<"Author Name"<<setw(20)<<"Book Category"<<setw(10)<<"Quantity"<<endl;

    for(int i=0;i<userHistory.size();i++){
        cout<<setw(20)<<trim(userHistory[i][1])<<setw(15)<<trim(userHistory[i][3])<<setw(35)<<trim(userHistory[i][4]);
        cout<<setw(25)<<trim(userHistory[i][5])<<setw(20)<<trim(userHistory[i][6])<<setw(10)<<trim(userHistory[i][7])<<endl;
    }
}

int partition(vector<vector<string>> &data,int low,int high,int columnIndex){
    string pivot=data[high][columnIndex];
    int i=low-1;

    for(int j=low;j<high;++j){
        if(data[j][columnIndex]<=pivot){
            ++i;
            swap(data[i],data[j]);
        }
    }
    swap(data[i+1],data[high]);
    return i+1;
}

void quickSort(vector<vector<string>> &data,int low,int high,int columnIndex){
    if(low<high){
        int pi=partition(data,low,high,columnIndex);

        quickSort(data,low,pi-1,columnIndex);
        quickSort(data,pi+1,high,columnIndex);
    }
}


void printTransaction(string username,int target,string returnDue){
    int columnIndex=1;
    vector<vector<string>> history=readCSV("TransactionHistory.csv");
    vector<vector<string>> userHistory;
    string receipt=to_string(target);

    for(int i=0;i<history.size();i++){
        if(history[i][columnIndex]==receipt){
            userHistory.push_back(history[i]);
        }
    }

    quickSort(userHistory,0,userHistory.size()-1,3);

    cout<<"\n-----Transaction Receipt-----"<<endl;
    cout<<"Receipt No. : "<<target<<endl;
    cout<<"User ID     : "<<username<<endl;
    cout<<"\nBorrowed Book Details"<<endl;

    for(int i=0;i<userHistory.size();i++){
        printRow(userHistory,i,3);
    }

    cout<<"Return Date: "<<returnDue<<endl;
    cout<<"*Kindly remember to return books on time."<<endl;
}

void searchBook(string m_id, string filename)
{
    vector<vector<string>> bookData = readCSV(filename);
    sortByColumn(bookData,0);

    int left = 0, right = bookData.size() - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (trim(bookData[mid][0]) == m_id)
        {
            printRow(bookData,mid,0);
            return;
        }
        else if (trim(bookData[mid][0]) < m_id){
            left = mid + 1;
        }
        else{
            right = mid - 1;
        }
    }

    cout << "Book not found!" << endl;
}


void Borrow(string username,int receipt,string returnDue){
    int columnIndex=0;
    int rowIndex=-1;
    int borrowChoice,quantity,choice,borrowQuantity=0;
    string bookID;
    vector<vector<string>> bookData=readCSV("BookDetails.csv");

    do{
        cout<<"Enter the Book ID: ";
        cin.ignore();
        getline(cin,bookID);

        for(int i=0;i<bookData.size();i++){
            if(bookData[i][columnIndex]==bookID){
                rowIndex=i;
                break;
            }
        }

        if(rowIndex==-1){
            cout<<"Book not found"<<endl;
            return;
        }

        do{
            cout<<"Enter the quantity: ";
            cin>>quantity;
            while(!validInt(quantity) || quantity<=0){
                cout<<"Wrong input. Please enter again: ";
                cin>>quantity;
            }

            int available=stoi(bookData[rowIndex][4]);
            if(available<quantity){
                cout<<"Invalid Quantity."<<endl;
                cout<<"The remaining quantity of this book is "<<available<<".\n";
                cout<<"Please enter again."<<endl<<endl;
            }
            else{
                cout<<"The book is available to borrow.\n";
                cout<<"Do you want to borrow? (1 for Yes / 0 for No) "<<endl;
                cin>>borrowChoice;

                if(borrowChoice==1){
                    // Open the transaction history file
                    fstream file2;
                    file2.open("TransactionHistory.csv",ios::in|ios::out|ios::app);
                    if(!file2){
                        cout<<"Error: Unable to open file."<<endl;
                        return;
                    }

                    // Write the new history into the file
                    file2<<username<<","<<receipt<<","<<returnDue<<","<<bookData[rowIndex][0]<<","<<bookData[rowIndex][1]<<",";
                    file2<<bookData[rowIndex][2]<<","<<bookData[rowIndex][3]<<","<<quantity<<endl;
                    file2.close();
                    updateQuantity(bookID,quantity,"BookDetails.csv");
                    borrowQuantity+=quantity;
                    break;
                }
                else{
                    cout<<"Borrowing Canceled."<<endl;
                    break;
                }
            }
        }while(true);
        cout<<"Do you want to borrow another book? (1 for Yes / 0 for No) "<<endl;
        cin>>choice;
    }while(choice==1);

    if(borrowQuantity!=0)
        printTransaction(username,receipt,returnDue);
}

int main()
{
    int choice,loginChoice,quantity;
    string bookname,bookID;
    string username;
    string bookFile="BookDetails.csv";
    string historyFile="TransactionHistory.csv";

    while(true){
        cout<<"Library Management System"<<endl;
        cout<<"Login..."<<endl;
        cout<<"1. User (Student, faculty, or staff)"<<endl;
        cout<<"2. Admin"<<endl;
        cout<<"3. Exit"<<endl;

        do{
            cout<<"Enter your choice: ";
            cin>>loginChoice;

            if(!validInt(loginChoice)){
                cout<<"Please enter an integer. "<<endl;
                loginChoice=-1;
            }
        }while(loginChoice==-1);


        switch(loginChoice){
        case 1:{
            bool libraryUser=true;
            cout<<"Enter your User ID: ";
            cin>>username;

            while(libraryUser){
                cout<<"\nWelcome to Library System, User!"<<endl;
                cout<<"1. Search a Book"<<endl;
                cout<<"2. Borrow Books"<<endl;
                cout<<"3. View Transaction History"<<endl;
                cout<<"4. Return to Main"<<endl;

                do{
                    cout<<"Enter your choice: ";
                    cin>>choice;

                    if(!validInt(choice)){
                        cout<<"Please enter an integer. "<<endl;
                        choice=-1;
                    }
                }while(choice==-1);

                switch(choice){
                    case 1:
                        cout<<"Search a Book..."<<endl;
                        cout<<"Please enter the book title or part of the title: "<<endl;
                        cin.ignore();
                        getline(cin,bookname);
                        searchBookByTitle(bookFile,bookname);
                        break;

                    case 2:{
                        int choice2;
                        int receipt=randomReceiptNumber();
                        string returnDue=getReturnDate();
                        cout<<"Borrow Books..."<<endl;
                        Borrow(username,receipt,returnDue);
                        break;
                    }

                    case 3:
                        cout<<"\nView Transaction History..."<<endl;
                        TransactionHistory(username);
                        break;

                    case 4:
                        cout<<"Exiting..."<<endl<<endl;
                        libraryUser=false;
                        break;

                    default:
                        cout<<"Invalid Choice.\n"<<endl;
                        break;
                }
            }
            break;
        }


        case 2:{
            string un,pw;
            cout<<"\nUser Name : ";
            cin>>un;
            cout<<"Password  : ";
            cin>>pw;

            bool Admin=authenticateAdmin(un,pw);

            while(Admin){
                cout<<"\nWelcome to Library System, Admin!"<<endl;
                cout<<"1. Add Book"<<endl;
                cout<<"2. Search Book"<<endl;
                cout<<"3. Edit Book Details"<<endl;
                cout<<"4. Delete Book"<<endl;
                cout<<"5. View All Books"<<endl;
                cout<<"6. Display Sorted Book List (sorted by ID)"<<endl;
                cout<<"7. Display Sorted Book List (sorted by Title)"<<endl;
                cout<<"8. View Transaction Receipt (sorted by receipt number)"<<endl;
                cout<<"9. Search Transaction Receipt by User ID"<<endl;
                cout<<"10. Return To Main"<<endl;

                do{
                    cout<<"Enter your choice: ";
                    cin>>choice;

                    if(!validInt(choice)){
                        cout<<"Please enter an integer. "<<endl;
                        choice=-1;
                    }
                }while(choice==-1);

                switch(choice){
                case 1:{
                    string id,title,author,category;
                    int Bquantity;

                    cout<<"Add Book..."<<endl;
                    cout<<"Enter Book ID: ";
                    cin>>id;
                    cout<<"Enter Book Title: ";
                    cin.ignore();
                    getline(cin,title);
                    cout<<"Enter Author Name: ";
                    getline(cin,author);
                    cout<<"Enter Book Category: ";
                    getline(cin,category);

                    cout<<"Enter Quantity: ";
                    cin>>Bquantity;
                    while (!validInt(Bquantity) || Bquantity <= 0) {
                        cout << "Wrong input. Please enter again: ";
                        cin >> Bquantity;
                    }

                    addBook(id,title,author,category,Bquantity,bookFile);

                    break;
                }

                case 2:{
                    string id;
                    cout<<"Enter Book ID to search: ";
                    cin>>id;
                    searchBook(id,bookFile);
                    break;
                }

                case 3:{
                    string id, title, author, category;
                    int available;
                    cout<<"Enter Book ID to edit: ";
                    cin>>id;
                    cout<<"Enter new Book Title: ";
                    cin.ignore();
                    getline(cin,title);
                    cout<<"Enter new Author Name: ";
                    getline(cin,author);
                    cout<<"Enter new Book Category: ";
                    getline(cin,category);

                    cout<<"Enter the quantity: ";
                    cin>>available;
                    while (!validInt(quantity) || quantity <= 0) {
                        cout << "Wrong input. Please enter again: ";
                        cin >> quantity;
                    }

                    editBookDetails(id,title,author,category,available,bookFile);
                    break;
                }

                case 4:{
                    string id;
                    cout<<"Enter Book ID to delete: ";
                    cin>>id;
                    deleteBook(id,bookFile);
                    break;
                }

                case 5:
                    viewAllBooks(bookFile);
                    break;

                case 6:
                    sortBooksByID(bookFile);
                    break;

                case 7:
                    sortBooksByTitle(bookFile);
                    break;

                case 8:
                    transactionReceipt();
                    break;

                case 9:{
                    string userId;
                    cout<<"Enter User ID to search transaction receipt: ";
                    cin>>userId;
                    searchTransactionByUserID(historyFile,userId);
                    break;
                }

                case 10:
                    cout<<"Exiting...\n"<<endl;
                    Admin=false;
                    break;

                default:
                    cout<<"Invalid Choice."<<endl;
                    break;

                }
            }
            break;
        }

        case 3:
            cout<<"Exiting..."<<endl<<endl;
            return false;

        default:
            cout<<"Invalid Choice."<<endl;
            break;
        }
    }

    return 0;
}

