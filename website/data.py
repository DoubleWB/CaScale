from werkzeug.security import generate_password_hash
from werkzeug.security import check_password_hash
from pymongo import MongoClient

collection = MongoClient()
db = collection.test

class User():

    def __init__(self, username):
        self.username = username

    def is_authenticated(self):
        return True

    def is_active(self):
        return True

    def is_anonymous(self):
        return False

    def get_id(self):
        return self.username

    @staticmethod
    def validate_login(password_hash, password):
        return check_password_hash(password_hash, password)

def load_user(username):  
    u = app.config['USERS_COLLECTION'].find_one({"_id": username})
    if not u:
        return None
    return User(u['_id'])

def create_account(usr, passw):
    user = usr
    password = passw
    pass_hash = generate_password_hash(password, method='pbkdf2:sha256')
    try:
        users = db.users
        users.insert_one({"_id": user, "password": pass_hash})
        print "User created."
    except DuplicateKeyError:
        print "User already present in DB."

def login(usr, passw):
    user = app.config['USERS_COLLECTION'].find_one({"_id": form.username.data})
        if user and User.validate_login(user['password'], form.password.data):
            user_obj = User(user['_id'])
            login_user(user_obj)
            flash("Logged in successfully!", category='success')
            return redirect(request.args.get("next") or url_for("write"))
        flash("Wrong username or password!", category='error')	

def main():
    # Ask for data to store
    user = raw_input("Enter your username: ")
    password = raw_input("Enter your password: ")
    create_account(user, password)
    # Insert the user in the DB


if __name__ == '__main__':
    main()
