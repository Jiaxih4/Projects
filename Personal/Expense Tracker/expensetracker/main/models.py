from django.db import models
from django.contrib.auth.models import User

# Create your models here.
class Expenses(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE, related_name="expenses", null=True)
    date = models.DateField()
    place = models.CharField(max_length = 200)
    purchase = models.CharField(max_length = 200)
    cost = models.DecimalField(max_digits = 99, decimal_places = 2)
    type = models.CharField(max_length = 200)
    description = models.CharField(max_length = 1000)

    def __str__(self):
        return self.purchase