# FOODIES

#### Video Demo: https://www.youtube.com/watch?v=J_cNbZUDsBE

#### Description: FOODIES is a recipe search web application that allow user to find recipes base on specific ingredients, cuisine type and diet. All recipes are extracted from [Spoonacular API](https://spoonacular.com/food-api).

# Technologies

- Flask
- Python
- SQLite
- HTML
- CSS
- Bootstrap 5

# Features

## Register Page

![Register page](./project/static/images/register.png)

In order to access website user have to register by providing a username and a confirmed password. Both username and password are stored into a database.

## Login Page

![Login page](./project/static/images/login.png)

Once registered, user will have to login and must provide username and password matching with database.

## Index Page

![Index page](./project/static/images/index.png)

When accessing main page, username is display to welcome user.

User must provide one or more ingredients in the text field. If not a flash notification will ask user to prompt for a query. Some checklists are available for more specific searches such as meal type, cuisine type and diet type. Inputs will be process on the back-end side and a response from the API as JSON data will be receive.

- ### Text Input

User must provide at least one ingredient or query to get to results page. Otherwise, user will get a flash notification asking them to provide input.

- ### Checklists

Some checklists are available to get more precise results. There is three subdivisions checklists as describe below.

1. #### Meal Type

   - Breakfast
   - Main Course
   - Desserts

   ![Meal Type](./project/static/images/mealtype.png)

2. #### Cuisine Type

    - African
    - American
    - British
    - Caribbean
    - Chinese
    - French
    - German
    - Greek
    - Indian
    - Irish
    - Italian
    - Japanese
    - Jewish
    - Korean
    - Latin American
    - Mediterranean
    - Mexican
    - Nordic
    - Spanish
    - Thaï
    - Vietnamese

   ![Cuisine Type](./project/static/images/cuisinetype.png)

3. #### Diet Type

    - Gluten Free
    - Vegetarian
    - Vegan

   ![Diet Type](./project/static/images/diettype.png)

## Recipes Result Page

![Result page](./project/static/images/recipes_1.png)

Recipes that match user's requirements will display in the result page. User can see recipe's name, a descriptive image, an approximate cooking time and servings.

![No Recipes](./project/static/images/no_results.png)

If there is no recipes matching user requirements then a message will be display asking user to try again.

## Modal

![Modal](./project/static/images/modal.png)

[Bootstrap's modal](https://getbootstrap.com/docs/5.0/components/modal/) is used to show more information of the recipes such as ingredients and steps to follow.

From modal's recipe, user can either go to the recipe's source page or choose to save the recipe. In that case "My Recipes" page will be loaded displaying all saved recipes and a flash notification that the recipe has been saved will appear.

If current recipe is already saved then user will get a flash notifiaction and will stay on results page.

## Save Recipes

![My Recipes](./project/static/images/myrecipes.png)

User's saved recipes will be display into "My Recipes" tab. From this tab, user will have access to the same informations that were display from results page. User can easily remove any recipes from modal button.

![No Saved Recipe](./project/static/images/no_saved.png)

Until user save his first recipe, a message will be display.

## Set New Password

![New Password](./project/static/images/password.png)

From "Set New Password" tab, users can set new password. Old password must be provide and new password have to be confirm twice to be accepted. In case of wrong inputs, users will get a flash notification.
