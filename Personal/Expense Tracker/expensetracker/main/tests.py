from django.test import TestCase
from .models import Expenses
from datetime import date

class ExpensesTestCase(TestCase):
    def setUp(self):
        self.expense = Expenses.objects.create(
            date=date.today(),
            place="Grocery Store",
            purchase="Milk",
            cost=2.50,
            type="Groceries",
            description="Bought milk from the grocery store"
        )

    def test_expense_created_successfully(self):
        """
        Test to ensure that an expense object is created successfully.
        """
        self.assertEqual(self.expense.place, "Grocery Store")
        self.assertEqual(self.expense.purchase, "Milk")
        self.assertEqual(self.expense.cost, 2.50)
        self.assertEqual(self.expense.type, "Groceries")
        self.assertEqual(self.expense.description, "Bought milk from the grocery store")

    def test_expense_date_field(self):
        """
        Test to ensure that the date field is correctly set.
        """
        self.assertEqual(self.expense.date, date.today())

    def test_expense_cost_decimal_places(self):
        """
        Test to ensure that the cost field is correctly set with decimal places.
        """
        self.assertEqual(self.expense.cost, 2.50)

    def test_expense_str_representation(self):
        """
        Test to ensure the string representation of an expense object is correct.
        """
        self.assertEqual(str(self.expense), "Expense at Grocery Store on {} - $2.50".format(date.today()))
    
    def test_delete_expense(self):
        """
        Test to ensure that an expense can be deleted.
        """
        expense_id = self.expense.id
        self.expense.delete()
        with self.assertRaises(Expenses.DoesNotExist):
            Expenses.objects.get(id=expense_id)

    def test_edit_expense(self):
        """
        Test to ensure that an expense's attributes can be edited correctly.
        """
        self.expense.place = "Supermarket"
        self.expense.save()
        edit_expense = Expenses.objects.get(id=self.expense.id)
        self.assertEqual(edit_expense.place, "Supermarket")

    def test_copy_expense_post(self):
        """
        Test copying an expense through the index view using a POST request.
        """
        self.client.post('/', {'Copy': 'Submit', 'c' + str(self.expense.id): 'clicked'})
        self.assertEqual(Expenses.objects.count(), 2)
        copied_expense = Expenses.objects.exclude(id=self.expense.id).get()
        self.assertEqual(copied_expense.purchase, 'Milk')