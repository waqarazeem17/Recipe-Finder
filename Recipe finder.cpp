#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include<algorithm>
#include <map>
#include <iomanip> // For std::setw
#include <sstream> // For std::stringstream


using namespace std;


struct Recipe {
    string name;
    int ingredients;
    vector<string> ingredientList;
    vector<string> dietTypes;  // Changed to a vector to store multiple diet types
    vector<string> methodSteps;

    Recipe(string n, int ing) : name(n), ingredients(ing) {}
};

// Node structure for the tree
struct TreeNode {
    string name; //   cuisine
    map<string, TreeNode*> children; // Nested structure for cuisines or recipes
    vector<Recipe> recipes; // Recipes for the specific cuisine

    TreeNode(string n) : name(n) {}
};

// Recipe Manager class
class RecipeManager {
    TreeNode* root; // Use raw pointer for root
public:
    RecipeManager() {
        root = new TreeNode("Root");
        initializeTree();
        loadFromFile();
    }

    ~RecipeManager() {
        saveToFile();
        delete root; // Clean up memory
    }

    void initializeTree() {
        vector<string> cuisines = {"American", "English", "Chinese", "Thai", "Italian", "Indian", "Middle Eastern", "Japanese", "Canadian", "Korean", "Spanish", "Mexican", "French"};

        for (const string& cuisine : cuisines) {
            root->children[cuisine] = new TreeNode(cuisine);
        }
    }

  void saveToFile() {
    ofstream outFile("recipes_finder.txt");
    if (!outFile) {
        cerr << "Error saving data to file.\n";
        return;
    }

    // Iterate through each cuisine and its recipes
    for (const auto& cuisinePair : root->children) {
        for (const auto& recipe : cuisinePair.second->recipes) {
            outFile << "Cuisine: " << cuisinePair.first << "\n"
                    << "Name: " << recipe.name << "\n"
                    << "Diet Type: ";

            // Save diet types (comma-separated)
            for (size_t i = 0; i < recipe.dietTypes.size(); ++i) {
                outFile << recipe.dietTypes[i];
                if (i != recipe.dietTypes.size() - 1) {
                    outFile << ", ";
                }
            }

            outFile << "\nIngredients: ";
            // Save ingredients (comma-separated)
            for (size_t i = 0; i < recipe.ingredientList.size(); ++i) {
                outFile << recipe.ingredientList[i];
                if (i != recipe.ingredientList.size() - 1) {
                    outFile << ", ";
                }
            }

            outFile << "\n--Instructions:--\n";
            // Save method steps (each on a new line)
            for (const auto& step : recipe.methodSteps) {
                outFile << step << "\n";
            }

            outFile << "\n";  // Blank line after each recipe
        }
    }

    outFile.close();
    //cout << "Recipes saved successfully.\n";  // Debugging message
}
void loadFromFile() {
    ifstream inFile("recipes_finder.txt");
    if (!inFile) {
        cerr << "No existing data found.\n";
        return;
    }

    string line, cuisine, name, dietList, ingredientList, methodSteps;
    while (getline(inFile, line)) {
        // Skip empty lines and continue
        if (line.empty()) continue;

        // Read the cuisine
        if (line.find("Cuisine: ") == 0) {
            cuisine = line.substr(9);
        }
        // Read the name of the recipe
        else if (line.find("Name: ") == 0) {
            name = line.substr(6);
        }
        // Read the diet type(s)
        else if (line.find("Diet Type: ") == 0) {
            dietList = line.substr(11);
        }
        // Read the ingredients list
        else if (line.find("Ingredients: ") == 0) {
            ingredientList = line.substr(13);
        }
        // Read the method steps
        else if (line.find("--Instructions:--") == 0) {
            methodSteps.clear();  // Clear previous method steps
            while (getline(inFile, line) && !line.empty()) {
                methodSteps += line + "~"; // Store each step with a delimiter
            }
        }

        // Check if we have reached the end of a recipe
        if (line.empty() || inFile.eof()) {
            if (!name.empty() && !cuisine.empty() && !ingredientList.empty() && !methodSteps.empty()) {
                Recipe newRecipe(name, 0);  // Initialize a recipe with name and ingredient count 0

                // Parse diet list
                stringstream dietStream(dietList);
                string diet;
                while (getline(dietStream, diet, ',')) {
                    if (!diet.empty()) {
                        newRecipe.dietTypes.push_back(diet);
                    }
                }

                // Parse ingredients list
                stringstream ingredientStream(ingredientList);
                string ingredient;
                while (getline(ingredientStream, ingredient, ',')) {
                    if (!ingredient.empty()) {
                        newRecipe.ingredientList.push_back(ingredient);
                        newRecipe.ingredients++;  // Increment ingredient count
                    }
                }

                // Parse method steps
                stringstream methodStream(methodSteps);
                string step;
                while (getline(methodStream, step, '~')) {
                    if (!step.empty()) {
                        newRecipe.methodSteps.push_back(step);
                    }
                }

                // Add the new recipe to the appropriate cuisine
                if (root->children.find(cuisine) == root->children.end()) {
                    root->children[cuisine] = new TreeNode(cuisine);
                }
                root->children[cuisine]->recipes.push_back(newRecipe);
            }

            // Reset fields for the next recipe
            name.clear();
            dietList.clear();
            ingredientList.clear();
            methodSteps.clear();
            cuisine.clear();
        }
    }

    // Process the last recipe if the file does not end with a blank line
    if (!name.empty() && !cuisine.empty() && !ingredientList.empty() && !methodSteps.empty()) {
        Recipe newRecipe(name, 0);
        // Parse diet list
        stringstream dietStream(dietList);
        string diet;
        while (getline(dietStream, diet, ',')) {
            if (!diet.empty()) {
                newRecipe.dietTypes.push_back(diet);
            }
        }

        // Parse ingredients list
        stringstream ingredientStream(ingredientList);
        string ingredient;
        while (getline(ingredientStream, ingredient, ',')) {
            if (!ingredient.empty ()) {
                newRecipe.ingredientList.push_back(ingredient);
                newRecipe.ingredients++;  // Increment ingredient count
            }
        }

        // Parse method steps
        stringstream methodStream(methodSteps);
        string step;
        while (getline(methodStream, step, '~')) {
            if (!step.empty()) {
                newRecipe.methodSteps.push_back(step);
            }
        }

        // Add the new recipe to the appropriate cuisine
        if (root->children.find(cuisine) == root->children.end()) {
            root->children[cuisine] = new TreeNode(cuisine);
        }
        root->children[cuisine]->recipes.push_back(newRecipe);
    }

    inFile.close();
   // cout << "Recipes loaded successfully.\n";  // Debugging message
} 

    bool adminLogin() {
        string username, password;
        int attempts = 3; // Allow the admin to try 3 times

        while (attempts > 0) {
        	cout<< "\t---------------------"<<endl;
            cout << "\t         Login          " << endl;
            cout<< "\t---------------------"<<endl;
			cout << "\nEnter Admin Username: ";
			 cin >> username;
            cout << "Enter Password: ";
            cin >> password;

            if (username == "admin" && password == "123456") {
                cout << "Login successful!\n";
                return true; // Admin login successful
            } else {
                attempts--; // Decrease the attempts
                if (attempts > 0) {
                    cout << "Incorrect Username or Password. You have " << attempts << " attempts left.\n";
                } else {
                    cout << "Too many failed login attempts. Exiting...\n";
                    return false; // Exit after 3 failed attempts
                }
            }
        }
        return false; // Return false after exhausting attempts
    }

    void addRecipe() {
        string cuisine, name;
        int ingredients;
        vector<string> ingredientList;
        vector<string> dietTypes;
        vector<string> methodSteps; // To store the multiple dietary restrictions

        cout << "\n---Cuisines---"; 
        cout << "\n~ American\n~ English\n~ Chinese\n~ Thai\n~ Italian\n~ Indian\n~ Middle Eastern\n~ Japanese\n~ Canadian\n~ Korean\n~ Spanish\n~ Mexican\n~ French\n\nSelect: ";
        
        cin.ignore(); 
        getline(cin, cuisine);
        if (root->children.find(cuisine) == root->children.end()) {
             root->children[cuisine] = new TreeNode(cuisine);
        }

        // Enter recipe details
        cout << "Enter dish name: ";
        getline(cin, name);
        cout << "Enter number of ingredients: ";
        cin >> ingredients;
        cin.ignore();  // Clear input buffer
        cout << "Enter each ingredient (press Enter after each):\n";
        for (int i = 0; i < ingredients; ++i) {
            string ingredient;
            cout << i + 1 << ") ";
            getline(cin, ingredient);
            ingredientList.push_back(ingredient);
        }

        // Admin enters dietary restrictions (multiple options can be selected)
        cout << "\n Dietary Restriction(s)\n";
        cout << "1. Lactose-Free\n";
        cout << "2. Gluten-Free\n";
        cout << "3. Vegetarian\n";
        cout << "4. Low Carb\n";
        cout << "5. High Protein\n";
        cout << "6. Diabetic-Friendly\n";
        cout << "7. Low Sodium\n";
        cout << "8. Nut-Free\n";
        cout << "9. Keto\n";
        cout << "select dietary restrictions for this recipe (select multiple by entering numbers separated by space): ";
        
        // Read in multiple choices from the admin (space-separated)
        string restrictionInput;
        getline(cin, restrictionInput);
        stringstream ss(restrictionInput);
        int choice;
        while (ss >> choice) {
            switch (choice) {
                case 1: dietTypes.push_back("Lactose-Free"); break;
                case 2: dietTypes.push_back("Gluten-Free"); break;
                case 3: dietTypes.push_back("Vegetarian"); break;
                case 4: dietTypes.push_back("Low Carb"); break;
                case 5: dietTypes.push_back("High Protein"); break;
                case 6: dietTypes.push_back("Diabetic-Friendly"); break;
                case 7: dietTypes.push_back("Low Sodium"); break;
                case 8: dietTypes.push_back("Nut-Free"); break;
                case 9: dietTypes.push_back("Keto"); break;
                default:
                    cout << "Invalid choice! Skipping...\n";
                    break;
            }
        }
        cout << "\nEnter the number of steps for the instructions: ";
        int steps;
        cin >> steps;
        cin.ignore(); // Clear input buffer

        cout << "\nEnter the instructions (press Enter after each step):\n";
        for (int i = 1; i <= steps; ++i) {
            string step;
            cout << "Step " << i << ": ";
            getline(cin, step);
            methodSteps.push_back(step);
        }

        // Add the recipe to the corresponding cuisine in the tree
        Recipe newRecipe(name, ingredients);
        newRecipe.ingredientList = ingredientList;
        newRecipe.dietTypes = dietTypes;  // Save dietary restrictions
        newRecipe.methodSteps = methodSteps;
        root->children[cuisine]->recipes.push_back(newRecipe);
        saveToFile();
        cout << "\n---Recipe added successfully!---\n";
    }

    void deleteRecipe() {
        string cuisine, dishName;

        cout << "---Cuisines---"; 
        cout << "\n~ American\n~ English\n~ Chinese\n~ Thai\n~ Italian\n~ Indian\n~ Middle Eastern\n~ Japanese \n~ Canadian\n~ Korean\n~ Spanish\n~ Mexican\n~ French\n\nSelect: ";
        
        cin.ignore(); // Clear the input buffer
        getline(cin, cuisine);

        // Check if the cuisine exists
        if (root->children.find(cuisine) == root->children.end()) {
            cout << "\nCuisine not found! Exiting...\n";
            return; // Exit if the cuisine does not exist
        }

        // Select recipe to delete
        cout << "\nEnter dish name to delete: ";
        getline(cin, dishName); // No need for another cin.ignore() here

        vector<Recipe>& recipes = root->children[cuisine]->recipes;

        // Debugging: Print available recipes
        cout << "\nAvailable recipes in " << cuisine << ":\n";
        cout << "-------------------------------------------\n";
        for (const auto& recipe : recipes) {
            cout << "- " << recipe.name << "\n"; // List all recipes
        }

        // Use a simple loop to find and delete the recipe
        for (auto it = recipes.begin(); it != recipes.end(); ++it) {
            if (it->name == dishName) { // Exact match
                recipes.erase(it);
                cout << "---Recipe deleted successfully!---\n";
                saveToFile();
                return;
            }
        }

        cout << "---Recipe not found!---\n";
    }

    void updateRecipe() {
        string cuisine, dishName;

        // Select cuisine
        cout << "---Cuisines---"; 
        cout << "\n~ American\n~ English\n~ Chinese\n~ Thai\n~ Italian\n~ Indian\n~ Middle Eastern\n~ Japanese\n~ Canadian\n~ Korean\n~ Spanish\n~ Mexican\n~ French\n\nSelect: ";
        
        cin.ignore(); // Clear any leftover input buffer
        getline(cin, cuisine);

        if (root->children.find(cuisine) == root->children.end()) {
          root->children[cuisine] = new TreeNode(cuisine);// If cuisine doesn't exist, create it
        }

        // Select recipe to update
        cout << "\nEnter dish name to update: ";
        getline(cin, dishName);

        vector<Recipe>& recipes = root->children[cuisine]->recipes;
        bool recipeFound = false;
        int choice; // Declare choice here

        for (auto& recipe : recipes) {
            if (recipe.name == dishName) {
                cout << "Updating recipe " << recipe.name << "\n";
                cout << "-------------------------------------\n";

                // Update dish name
                cout << "\nEnter new dish name: ";
                getline(cin, recipe.name);

                // Update ingredients
                cout << "\nEnter new number of ingredients: ";
                cin >> recipe.ingredients;
                cin.ignore(); // Clear input buffer
                recipe.ingredientList.clear(); // Clear existing ingredients
                cout << "\nEnter each new ingredient (press Enter after each):\n";
                for (int i = 0; i < recipe.ingredients; ++i) {
                    string ingredient;
                    cout << i + 1 << ") ";
                    getline(cin, ingredient);
                    recipe.ingredientList.push_back(ingredient);
                }

                // Update dietary restrictions
                cout << "---Dietary Restrictions---\n";
                cout << "1. Lactose-Free\n2. Gluten-Free\n3. Vegetarian\n4. Low Carb\n5. High Protein\n6. Diabetic-Friendly\n7. Low Sodium\n8. Nut-Free\n9. Keto\nEnter new dietary restrictions (select multiple by entering numbers separated by space): ";
                string restrictionInput;
                getline(cin, restrictionInput);
                stringstream ss(restrictionInput);
                recipe.dietTypes.clear(); // Clear existing dietary types
                while (ss >> choice) {
                    switch (choice) {
                        case 1: recipe.dietTypes.push_back("Lactose-Free"); break;
                        case 2: recipe.dietTypes.push_back("Gluten-Free"); break;
                        case 3: recipe.dietTypes.push_back("Vegetarian"); break;
                        case 4: recipe.dietTypes.push_back("Low Carb"); break;
                        case 5: recipe.dietTypes.push_back("High Protein"); break;
                        case 6: recipe.dietTypes.push_back("Diabetic-Friendly"); break;
                        case 7: recipe.dietTypes.push_back("Low Sodium"); break;
                        case 8: recipe.dietTypes.push_back("Nut-Free"); break;
                        case 9: recipe.dietTypes.push_back("Keto"); break;
                        default:
                            cout << "Invalid choice! Skipping...\n";
                            break;
                    }
                }

                // Update method steps (clear existing steps before adding new ones)
                cout << "\nEnter the number of instructions: ";

                int steps;
                cin >> steps;
                cin.ignore(); // Clear input buffer
                recipe.methodSteps.clear(); // Clear existing method steps

                cout << "\nEnter the instructions (press Enter after each step):\n";
                for (int i = 1; i <= steps; ++i) {
                    string step;
                    cout << "Step " << i << ": ";
                    getline(cin, step);
                    recipe.methodSteps.push_back(step);
                }

                cout << "---Recipe updated successfully!---\n";
                recipeFound = true;
                saveToFile(); // Save the updated recipe data to file
                break;
            }
        }

        if (!recipeFound) {
            cout << "---Recipe not found!---\n";
        }
    }

void displayRecipesByDiet() {
    string restriction;
    bool validRestriction = false;

    // Display available dietary restrictions
    cout << "---Dietary Restriction---\n";
    cout << "1. Lactose-Free\n";
    cout << "2. Gluten-Free\n";
    cout << "3. Vegetarian\n";
    cout << "4. Low Carb\n";
    cout << "5. High Protein\n";
    cout << "6. Diabetic-Friendly\n";
    cout << "7. Low Sodium\n";
    cout << "8. Nut-Free\n";
    cout << "9. Keto\n";
    cout << "Enter the number corresponding to your dietary restriction: ";
    
    int choice;
    cin >> choice;

    // Map the user's choice to a dietary restriction
    switch (choice) {
        case 1: restriction = "Lactose-Free"; validRestriction = true; break;
        case 2: restriction = "Gluten-Free"; validRestriction = true; break;
        case 3: restriction = "Vegetarian"; validRestriction = true; break;
        case 4: restriction = "Low Carb"; validRestriction = true; break;
        case 5: restriction = "High Protein"; validRestriction = true; break;
        case 6: restriction = "Diabetic-Friendly"; validRestriction = true; break;
        case 7: restriction = "Low Sodium"; validRestriction = true; break;
        case 8: restriction = "Nut-Free"; validRestriction = true; break;
        case 9: restriction = "Keto"; validRestriction = true; break;
        default:
            cout << "Invalid choice! Please select a valid option from the list.\n";
            return;
    }

    if (validRestriction) {
        cout << "\nDisplaying recipes for diet type: " << restriction << "\n";

        bool found = false;
        for (const auto& cuisinePair : root->children) {
            for (const auto& recipe : cuisinePair.second->recipes) {
                // Check if the recipe matches the selected dietary restriction
                if (find(recipe.dietTypes.begin(), recipe.dietTypes.end(), restriction) != recipe.dietTypes.end()) {
                    
					cout << string(120, '-') << "\n";
					// Display Cuisine and Dish Name
                    cout << "| Cuisine: " << left << setw(43) << cuisinePair.first
                         << " | Dish Name: " << recipe.name << "\n";
                    cout << string(120, '-') << "\n";
                    // Display Diet Type
                    cout << "| Diet Type: " << recipe.dietTypes.front() << "\n";
                    cout << string(120, '-') << "\n"; // Separator for header and content

                    // Display Ingredients and Instructions in a tabular format
                    cout << "| " << left << setw(33) << "Ingredients:"
                         << "| Instructions:\n";
                    cout << string(120, '-') << "\n";

                    // Iterate through ingredients and instructions to align them
                    size_t maxRows = max(recipe.ingredientList.size(), recipe.methodSteps.size());
                    for (size_t i = 0; i < maxRows; ++i) {
                        cout << "| ";
                        if (i < recipe.ingredientList.size()) {
                            // Indexed ingredients
                            cout << i + 1 << ". " << left << setw(30) << recipe.ingredientList[i];
                        } else {
                            cout << setw(33) << " "; // Empty space if no more ingredients
                        }

                        cout << "| ";
                        if (i < recipe.methodSteps.size()) {
                            // Instructions with step numbers
                            cout << "Step " << (i + 1) << ": " << recipe.methodSteps[i];
                        }
                        cout << "\n";
                    }

                    // Separator between recipes
                    cout << string(120, '-') << "\n";
                    found = true;
                }
            }
        }

        if (!found) {
            cout << "---No recipes match the selected dietary restriction.---\n";
        }
    }
}

string concatenateDietTypes(const vector<string>& dietTypes) {
    string result = "";
    for (size_t i = 0; i < dietTypes.size(); ++i) {
        result += dietTypes[i];
        if (i != dietTypes.size() - 1) {
            result += ", ";  // Add a comma between items, but not after the last one
        }
    }
    return result;
}
void displayRecipes() {
    bool recipesDisplayed = false;

    for (const auto& cuisinePair : root->children) {
        if (cuisinePair.second->recipes.empty()) continue;

        recipesDisplayed = true;

        // Print Cuisine Heading
        cout << "\n" << string(120, '-') << "\n";
        cout << "| Cuisine: " << left << setw(90) << cuisinePair.first << "\n";
        cout << string(120, '-') << "\n";

        // Iterate through recipes
        for (const auto& recipe : cuisinePair.second->recipes) {
            // Display Dish Name
            cout << "| Dish Name: " << left << setw(90) << recipe.name << "\n";

            // Separator between Dish Name and Diet Type
            cout << string(120, '-') << "\n";

            // Display Diet Type
            cout << "| Diet Type: " << left << setw(90) 
                 << (recipe.dietTypes.empty() ? "N/A" : concatenateDietTypes(recipe.dietTypes)) << "\n";

            // Separator before Ingredients and Instructions
            cout << string(120, '-') << "\n";

            // Print Ingredients and Instructions table headings
            cout << "| " << left << setw(33) << "Ingredients:"
                 << "| Instructions:\n";
            cout << string(120, '-') << "\n";

            // Display Ingredients and Instructions row by row
            size_t maxRows = max(recipe.ingredientList.size(), recipe.methodSteps.size());
            for (size_t i = 0; i < maxRows; ++i) {
                cout << "| ";
                if (i < recipe.ingredientList.size()) {
                    // Indexed ingredients
                    cout << i + 1 << ". " << left << setw(30) << recipe.ingredientList[i];
                } else {
                    cout << setw(33) << " "; // Empty space if no more ingredients
                }

                cout << "| ";
                if (i < recipe.methodSteps.size()) {
                    // Instructions with step numbers
                    cout << "Step " << (i + 1) << ": " << recipe.methodSteps[i];
                }
                cout << "\n";
            }

            // Separator between recipes
            cout << string(120, '-') << "\n";
        }
    }

    if (!recipesDisplayed) {
        cout << "\n---No recipes available to display---\n";
    }
}

void searchByIngredients() {
    int ingredientCount;
    vector<string> userIngredients;

    // User input for ingredients
    cout << "\nHow many ingredients do you have? ";
    cin >> ingredientCount;
    cin.ignore(); // Clear the input buffer

    cout << "\nEnter your ingredients:\n";
    for (int i = 0; i < ingredientCount; ++i) {
        string ingredient;
        cout << i + 1 << ") ";
        getline(cin, ingredient);
        userIngredients.push_back(ingredient);
    }

    // Search for matching recipes
    bool found = false;
    cout << "\nMatching Recipes:\n";

    for (const auto& cuisinePair : root->children) {
        for (const auto& recipe : cuisinePair.second->recipes) {
            // Check if all user ingredients match the recipe's ingredients
            bool allIngredientsMatch = true;

            for (const string& userIng : userIngredients) {
                string lowerUserIng = userIng;
                std::transform(lowerUserIng.begin(), lowerUserIng.end(), lowerUserIng.begin(), ::tolower);

                bool ingredientFound = false;
                for (const string& recipeIng : recipe.ingredientList) {
                    string lowerRecipeIng = recipeIng;
                    std::transform(lowerRecipeIng.begin(), lowerRecipeIng.end(), lowerRecipeIng.begin(), ::tolower);

                    if (lowerRecipeIng.find(lowerUserIng) != string::npos) {
                        ingredientFound = true;
                        break;
                    }
                }

                if (!ingredientFound) {
                    allIngredientsMatch = false;
                    break;
                }
            }

            // If all ingredients match, display the recipe
            if (allIngredientsMatch) {
                found = true;

                // Print Cuisine Heading
                cout << "\n" << string(120, '-') << "\n";
                cout << "| Cuisine: " << left << setw(90) << cuisinePair.first << "\n";
                cout << string(120, '-') << "\n";

                // Print Dish Name
                cout << "| Dish Name: " << left << setw(90) << recipe.name << "\n";
				cout << string(120, '-') << "\n";
                // Print Diet Type
                cout << "| Diet Type: " << left << setw(90)
                     << (recipe.dietTypes.empty() ? "N/A" : concatenateDietTypes(recipe.dietTypes)) << "\n";
                cout << string(120, '-') << "\n";

                // Print Ingredients and Instructions table headings
                cout << "| " << left << setw(33) << "Ingredients:"
                     << "| Instructions:\n";
                cout << string(120, '-') << "\n";

                // Display Ingredients and Instructions row by row
                size_t maxRows = max(recipe.ingredientList.size(), recipe.methodSteps.size());
                for (size_t i = 0; i < maxRows; ++i) {
                    cout << "| ";
                    if (i < recipe.ingredientList.size()) {
                        // Indexed ingredients
                        cout << i + 1 << ". " << left << setw(30) << recipe.ingredientList[i];
                    } else {
                        cout << setw(33) << " "; // Empty space if no more ingredients
                    }

                    cout << "| ";
                    if (i < recipe.methodSteps.size()) {
                        // Instructions with step numbers
                        cout << "Step " << (i + 1) << ": " << recipe.methodSteps[i];
                    }
                    cout << "\n";
                }

                // Separator after each recipe
                cout << string(120, '-') << "\n";
            }
        }
    }

    if (!found) {
        cout << "\n---No recipes match your criteria---\n";
    }
}

void viewByCuisine() {
    string cuisine;
    cout << "\nEnter cuisine to view recipes: ";
    cin.ignore();
    getline(cin, cuisine);

    if (root->children.find(cuisine) != root->children.end()) {
        cout << "\n" << string(120, '-') << "\n";
        cout << "| Cuisine: " << left << setw(107) << cuisine << "\n";
        cout << string(120, '-') << "\n";

        for (const auto& recipe : root->children[cuisine]->recipes) {
            // Print Dish Name
            cout << "| Dish Name: " << left << setw(105) << recipe.name << "\n";
			cout << string(120, '-') << "\n";
            // Print Diet Type
            cout << "| Diet Type: " << left << setw(105)
                 << (recipe.dietTypes.empty() ? "N/A" : concatenateDietTypes(recipe.dietTypes)) << "\n";

            // Separator before Ingredients and Instructions
            cout << string(120, '-') << "\n";

            // Print Ingredients and Instructions table headings
            cout << "| " << left << setw(33) << "Ingredients:"
                 << "| Instructions:\n";
            cout << string(120, '-') << "\n";

            // Display Ingredients and Instructions row by row
            size_t maxRows = max(recipe.ingredientList.size(), recipe.methodSteps.size());
            for (size_t i = 0; i < maxRows; ++i) {
                cout << "| ";
                if (i < recipe.ingredientList.size()) {
                    // Indexed ingredients
                    cout << i + 1 << ". " << left << setw(30) << recipe.ingredientList[i];
                } else {
                    cout << setw(33) << " "; // Empty space if no more ingredients
                }

                cout << "| ";
                if (i < recipe.methodSteps.size()) {
                    // Instructions with step numbers
                    cout << "Step " << (i + 1) << ": " << recipe.methodSteps[i];
                }
                cout << "\n";
            }

            // Separator after each recipe
            cout << string(120, '-') << "\n";
        }
    } else {
        cout << "\n---No recipes found for this cuisine---\n";
    }
}
};

int main() {
    RecipeManager manager;
    int role;
    do {
        cout << "\t\t-------------------------------------\n";
        cout << "\t\t  Welcome to the Recipe Dashboard!\n";
        cout << "\t\t-------------------------------------";
        cout << "\nSelect your role:\n";
        cout << "1. Admin\n";
        cout << "2. User\n";
        cout << "3. Exit\n";
        cout << "\nEnter your choice: ";
        cin >> role;

        if (role == 1) {
            if (manager.adminLogin()) {
                // Admin dashboard
                int choice;
                do {
                    cout << "\n-------------------------"<<endl;
                    cout << "      Admin Dashboard"<<endl;
                    cout << "-------------------------"<<endl;
                    cout << "1. Add Recipe\n";
                    cout << "2. Remove Recipe\n";
                    cout << "3. Update Recipe\n";
                    cout << "4. View Recipe\n";
                    cout << "5. Exit\n";
                    cout << "\nEnter your choice: ";
                    cin >> choice;

                    switch (choice) {
                        case 1:
                            manager.addRecipe();
                            break;
                        case 2:
                            manager.deleteRecipe();
                            break;
                        case 3:
                            manager.updateRecipe();
                            break;
                        case 4:
                            manager.displayRecipes();
                            break;
                        case 5:
                            cout << "Exiting Admin Dashboard.\n";
                            break;
                        default:
                            cout << "Invalid choice!\n";
                    }
                } while (choice != 5);
            }
        } else if (role == 2) {
            int choice;
            do {
                cout << "\n-------------------------"<<endl;
                cout << "      User  Dashboard"<<endl;
                cout << "-------------------------"<<endl;
                cout << "1. View Recipes by Ingredients\n";
                cout << "2. View Recipes by Cuisine\n";
                cout << "3. View Recipes by Diet\n";
                cout << "4. Exit\n";
                cout << "\nEnter your choice: ";
                cin >> choice;

                switch (choice) {
                    case 1:
                        manager.searchByIngredients();
                        break;
                    case 2:
                        manager.viewByCuisine();
                        break;
                    case 3:
                        manager.displayRecipesByDiet();
                        break;
                    case 4:
                        cout << "Exiting User Dashboard.\n";
                        break;
                    default:
                        cout << "Invalid choice!\n";
                }
            } while (choice != 4);
        } else if (role == 3) {
            cout << "Exiting app... Thank you!\n";
            exit(0);
        } else {
            cout << "Choose a valid number.\n";
        }
    } while (role != 3);
}
