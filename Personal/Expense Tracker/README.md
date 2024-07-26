# ExpenseTracker

## Introduction

### What is ExpenseTracker?

ExpenseTracker is a web-based application designed to help users manage their personal finances effectively. It provides a simple and user-friendly way to record, categorize, and analyze spending. Users can add, edit, copy, and delete expenses, while also viewing visual insights through automatically generated pie charts and bar graphs. This helps them quickly identify spending patterns and make informed budgeting decisions.

### Key Features
- **Expense Management:** Track and categorize expenses easily.
- **Visual Insights:** Instantly visualize data with pie charts and bar graphs.
- **Customizable:** Tailor categories and data to personal financial needs.
- **Secure and Scalable:** Protects user data and is accessible across devices.

## Technical Architecture

### Overview
ExpenseTracker follows a client-server architecture where the front end communicates with a Python/Django backend, which manages data processing and storage. The backend uses an SQLite relational database to securely store and manage user data.

### Architecture Components

1. **Frontend:**
   - Developed using **HTML** and **CSS** for a user-friendly, responsive design.
   - Allows users to submit expenses through intuitive forms and sends data to the backend via HTTP requests.
   - Receives processed data and displays it visually through dynamic graphs and charts.

2. **Backend:**
   - Built with **Python** and the **Django** framework, following a Model-View-Template (MVT) architecture.
   - Handles the CRUD operations required for expense management and user information.
   - Processes and generates visual representations of the expense data (pie charts and bar graphs) and returns them to the front end.
   - Manages user authentication with the built-in Django system, ensuring secure personalized access for each user.

3. **Database:**
   - **SQLite** relational database stores all user and expense data, leveraging Django's built-in ORM for efficient database management.
   - Isolates each user's expenses and links them securely to their authentication credentials.
   - Enables accurate, efficient querying and visualization of stored data.

### Data Flow Summary

1. **Expense Entry:** Users submit expenses via the front end.
2. **Data Storage:** Backend stores the expense in the SQLite database for the specific authenticated user.
3. **Data Processing:** Backend retrieves, processes, and generates visual insights.
4. **Data Display:** Frontend displays data to users via graphical charts and graphs.

### Diagram
<img width="592" alt="Screenshot 2024-05-05 at 5 41 11 PM" src="https://github.com/CS222-UIUC-SP24/group-project-team-68/assets/87674409/b712d433-e0f7-4153-bc40-89a55392b6cb">

## Group Members and Their Roles

- **Jiaxi Huang:**
  - Role: **Project Manager & Data Visualization Specialist**
  - Responsibilities:
    - Oversaw the project timeline and task delegation, ensuring the team's alignment with project goals and deadlines.
    - Designed and implemented the backend logic for data processing and conversion into graphical insights.
    - Conducted testing and debugging to ensure data accuracy in pie charts and bar graphs, providing users with actionable insights.

- **Uttkarsh Singh:** 
  - Role: **Backend Developer & Database Administrator**
  - Responsibilities:
    - Set up the backend server using Python and Django, implementing robust APIs for handling user submissions and processing expense data.
    - Managed the SQLite relational database, ensuring efficient data storage, indexing, and secure user authentication.
    - Coordinated with frontend developers to deliver processed data insights to the UI and ensure smooth user account management.

- **Apoorv Pitta:** 
  - Role: **Frontend Developer & UI/UX Designer**
  - Responsibilities: 
    - Developed the user interface using HTML and CSS to create an intuitive and accessible expense entry form.
    - Designed and implemented the visual data representations, including pie charts and bar graphs, ensuring data clarity and user-friendliness.
    - Collaborated with backend developers to seamlessly integrate user submissions and display dynamic expense data.

## Installation Instructions

### Prerequisites
- **Python:** Make sure you have Python installed. You can download it [here](https://www.python.org/downloads/).
- **Git:** Install Git for version control if not already available. Download it [here](https://git-scm.com/).

### Setting Up the Project

1. **Clone the Repository:** <br>
   Open your terminal (or command prompt on Windows) and run:
   ```bash
   git clone https://github.com/yourusername/expensetracker.git

2. **Navigate to the Project Directory** 
   ```bash
   cd expensetracker

3. **Set Up the Virtual Environment** <br>
   ```bash
   python -m venv venv

4. **Activate the Virtual Environment** <br>
   Windows:
   ```bash
      venv\Scripts\activate
   ```
   Linux/Mac:
   ```bash
      source venv/bin/activate
   ```
5. **Install the Dependencies** <br>
   ```bash
   pip install -r requirements.txt

6. **Database Migrations** <br>
   ```bash
   python manage.py makemigrations
   python manage.py migrate

7. **Create a Superuser Account (Admin)** <br>
   ```bash
   python manage.py createsuperuser

8. **Run the Development Server**
   ```bash
   python manage.py runserver

9. **Access ExpenseTracker in the browser** <br>
   ```bash
   Visit http://localhost:8000

