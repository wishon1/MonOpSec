import os
import openai
import ctypes
import tkinter as tk
from tkinter import messagebox

# Set up OpenAI API key
openai.api_key = os.getenv("OPENAI_API_KEY")  # Replace with your key directly if not using environment variable

# Load the compiled C library
lib = ctypes.CDLL('./libservicecheck.so')

# Function to check if the service is running
def check_service(service_name):
    # Call the C function and interpret its result
    result = lib.check_service_status(ctypes.c_char_p(service_name.encode()))
    return result == 0

# Function to get suggestions from ChatGPT
def get_suggestions(prompt):
    response = openai.Completion.create(
        engine="text-davinci-003",
        prompt=prompt,
        max_tokens=10,
        n=3,
        stop=None,
        temperature=0.5
    )
    return [choice.text.strip() for choice in response.choices]

# Function to handle search and display results
def search_service():
    service_name = entry.get()
    if service_name:
        is_running = check_service(service_name)
        if is_running:
            messagebox.showinfo("Service Status", f"The service '{service_name}' is running.")
        else:
            messagebox.showinfo("Service Status", f"The service '{service_name}' is not running.")
    else:
        messagebox.showwarning("Input Error", "Please enter a service name.")

# Function to update autocomplete suggestions
def update_suggestions(event):
    typed_text = entry.get()
    if typed_text:
        suggestions = get_suggestions(f"Suggest system services related to '{typed_text}'")
        suggestion_var.set(suggestions[0] if suggestions else "")

# GUI setup
root = tk.Tk()
root.title("Service Search")

# Input field for typing service names
entry = tk.Entry(root, width=40)
entry.pack(pady=10)
entry.bind("<KeyRelease>", update_suggestions)

# Variable to display autocomplete suggestion
suggestion_var = tk.StringVar()
suggestion_label = tk.Label(root, textvariable=suggestion_var, fg="grey")
suggestion_label.pack()

# Search button
search_button = tk.Button(root, text="Search Service", command=search_service)
search_button.pack(pady=10)

root.mainloop()
