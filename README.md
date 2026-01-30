# Library-Management-System  
A C++ console-based library management system designed to handle book borrowing, searching, and transaction history. This program supports both user (students, faculty, staff) and admin functionalities, with the ability to perform CRUD operations on books and transactions.  

## Features
### Users (Students, Faculty, or Staff):
- Search Books: Search for books by title or partial title.
- Borrow Books: Borrow books and get a transaction receipt.
- View Transaction History: View a user's transaction history (book borrowing details).

### Admin
- Add Book: Add a new book to the library.
- Search Book: Search for a book by its ID.
- Edit Book Details: Edit existing book details such as title, author, category, and quantity.
- Delete Book: Remove a book from the library.
- View All Books: Display all available books.
- Sort Books by ID: View books sorted by their ID.
- Sort Books by Title: View books sorted by their title.
- View Transaction Receipt: Display transaction receipts, sorted by receipt number.
- Search Transaction Receipt by User ID: Find transaction receipts based on user ID.

## File Structure
- **BookDetails.csv**: Stores information about books in the library (ID, Title, Author, Category, Quantity).
- **TransactionHistory.csv**: Keeps track of users' book borrowing activities (User ID, Receipt Number, Book ID, Book Title, Author, Category, Quantity, Return Due Date).
