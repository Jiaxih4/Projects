from django.shortcuts import render
from django.http import HttpResponse
from .models import Expenses
import numpy as np
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import seaborn as sns
import mpld3
from django.shortcuts import redirect
# Create your views here.

def index(response):
    if response.user.is_authenticated:

        ex = response.user.expenses.all()
        if response.method == "POST":
            if response.POST.get("Add Expense"):
                dat = response.POST.get("date")
                plac = response.POST.get("place")
                pur = response.POST.get("purchase")
                cos = response.POST.get("cost")
                typ = response.POST.get("type")
                des = response.POST.get("description")

                if len(plac) > 0 and len(pur) > 0 and len(typ) > 0 and len(des) > 0 and len(dat) > 0 and len(cos) > 0:
                    a = ex.create(date = dat, place = plac, purchase = pur, cost = cos, type = typ, description = des)
                    response.user.expenses.add(a)
            elif response.POST.get("Delete"):
                for item in ex.all():
                    if response.POST.get("c" + str(item.id)) == "clicked":
                        ex.filter(id = item.id).delete()
            elif response.POST.get("Copy"):
                for item in ex.all():
                    if response.POST.get("c" + str(item.id)) == "clicked":
                        temp = ex.get(id = item.id)
                        a = ex.create(date = temp.date, place = temp.place, purchase = temp.purchase, cost = temp.cost, type = temp.type, description = temp.description)
                        response.user.expenses.add(a)
                        break  # After copying, break out of the loop
            elif response.POST.get("Edit"):
                for item in ex.all():
                    if response.POST.get("c" + str(item.id)) == "clicked":
                        return redirect('description', ids=item.id)

        expenses_by_type = {}
        for expense in ex:
            expenses_by_type[expense.type] = expenses_by_type.get(expense.type, 0) + float(expense.cost)

        types = list(expenses_by_type.keys())
        totals = list(expenses_by_type.values())
        colors = sns.color_palette("hls", len(types))
        # Create a bar graph
        fig, ax = matplotlib.pyplot.subplots()
        bars = ax.bar(types, totals, color=colors)
        # Add labels to the x-axis
        ax.set_xticks(range(len(types)))
        ax.set_xticklabels(types, rotation=45)
        ax.set_xlabel('Expense Type')
        ax.set_ylabel('Total Money Spent')
        ax.set_title('Total Expenses by Type')
        ax.grid(True, linestyle='--', alpha=0.6)
        # Add data labels
        for bar in bars:
            yval = bar.get_height()
            matplotlib.pyplot.text(bar.get_x() + bar.get_width()/2, yval + 0.05, round(yval, 2), ha='center', va='bottom')
        # Convert the plot to HTML including titles and labels
        bar_graph = mpld3.fig_to_html(fig)
        matplotlib.pyplot.close(fig)

        # Create a pie chart
        fig, ax = matplotlib.pyplot.subplots()
        #explode = (0.1,) + (0,) * (len(types))
        ax.pie(totals, labels=types, autopct='%1.1f%%', startangle=140, shadow=True)
        ax.set_title('Total Expenses by Type')
        # Convert the plot to HTML including titles and labels
        pie_chart = mpld3.fig_to_html(fig)
        matplotlib.pyplot.close(fig)

        # Pass html_fig to the template
        return render(response, "main/index.html", {"ex": ex, 'bar': bar_graph, 'pie': pie_chart})
    return redirect("/login")

def description(response, ids):
    ex = Expenses.objects.all()
    obj = ex.get(id = ids)
    if response.method == "POST":
        # Implement the update logic if the form is submitted.
        obj.date = response.POST.get("date")
        obj.place = response.POST.get("place")
        obj.purchase = response.POST.get("purchase")
        obj.cost = response.POST.get("cost")
        obj.type = response.POST.get("type")
        obj.description = response.POST.get("description")
        obj.save()
        return redirect('/')  # Redirect to the index page
    else:
        # Render the page with the object's data filled in for editing
        return render(response, "main/description.html", {"obj": obj})
    
def help_info(request):
    return render(request, "main/help_info.html")