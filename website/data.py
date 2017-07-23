from werkzeug.security import generate_password_hash
from pymongo import MongoClient

collection = MongoClient()
db = collection.test

def create_account(usr, passw):
    user = usr
    password = passw
    pass_hash = generate_password_hash(password, method='pbkdf2:sha256')
    try:
        posts = db.posts
    posts.insert_one({"_id": user, "password": pass_hash})
        print "User created."
    except DuplicateKeyError:
        print "User already present in DB."

def main():
    # Ask for data to store
    #user = raw_input("Enter your username: ")
    #password = raw_input("Enter your password: ")
    #pass_hash = generate_password_hash(password, method='pbkdf2:sha256')
    # Insert the user in the DB


if __name__ == '__main__':
    main()
