from django.shortcuts import render


def home(request):
    context = {"message": "Hello!"}
    return render(request, "home.html", context)
