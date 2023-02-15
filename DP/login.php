<?php include "template.php"; ?>
<title>Log In</title>
<h1 class='text-primary'>Log In Below</h1>

<form action="index.php">
    <div class="form-group">
        <label for="email">Username:</label>
        <input type="email" class="form-control" id="email">
    </div>
    <div class="form-group">
        <label for="pwd">Password:</label>
        <input type="password" class="form-control" id="pwd">
    </div>
    <button type="submit" class="btn btn-default">Submit</button>
</form>

<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = sanitise_data($_POST['username']);
    $password = sanitise_data($_POST['password']);
    $hashed_password = password_hash($password, PASSWORD_DEFAULT);


}
