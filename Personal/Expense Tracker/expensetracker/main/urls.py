from django.urls import path, include
from . import views
from django.contrib import admin
from register import views as v

urlpatterns = [
    path('admin/', admin.site.urls),
    path("", views.index, name="index"),
    path("register/", v.register, name="register"),
    path("<int:ids>", views.description, name="description"),
    path('help/', views.help_info, name='help'),  # Add this line for the help/info page
    path('', include("django.contrib.auth.urls")),
]