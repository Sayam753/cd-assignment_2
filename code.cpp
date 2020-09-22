#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "tokens.h"

using namespace std;
extern int yylex();
extern int yylineno;
extern char *yytext;

class Production
{
public:
    string variable;
    vector<string> productions;
    bool is_left_refactored;
    static vector<string> unused_variable_names;

    Production(void)
    {
        is_left_refactored = false;
    }

    string complete_production(void)
    {
        string joined_productions = variable + " -> ";
        for (vector<string>::iterator pr = productions.begin(); pr != productions.end(); ++pr)
        {
            joined_productions += (*pr);
            if (pr != --productions.end())
            {
                joined_productions += " | ";
            }
        }
        return joined_productions;
    }
};
vector<string> Production::unused_variable_names;

void do_left_factoring(vector<Production> &all_productions)
{
    vector<Production> new_prods;
    for (vector<Production>::iterator it = all_productions.begin(); it != all_productions.end(); ++it)
    {
        if ((*it).is_left_refactored == 0)
        {
            // Check if EPSILON is there
            int is_epsilon = 0;
            if (find((*it).productions.begin(), (*it).productions.end(), "EPSILON") != (*it).productions.end())
            {
                is_epsilon = 1;
            }
            if ((*it).productions.size() == 1 && is_epsilon == 1)
            {
                (*it).is_left_refactored = true;
                continue;
            }
            else if (is_epsilon == 1)
            {
                (*it).productions.erase(remove((*it).productions.begin(), (*it).productions.end(), "EPSILON"), (*it).productions.end());
                (*it).productions.push_back("EPSILON");
            }

            // Finding common slices
            sort((*it).productions.begin(), (*it).productions.end() - is_epsilon);
            int production_size = (*it).productions.size();
            vector<pair<int, int> > common_section_slices;
            for (int index = 0; index < production_size - is_epsilon; index++)
            {
                int go_ahead = index;
                while (go_ahead < production_size - is_epsilon)
                {
                    if ((*it).productions[index][0] == (*it).productions[go_ahead][0])
                        go_ahead++;
                    else
                        break;
                }
                go_ahead = go_ahead - 1;
                if (go_ahead != index)
                {
                    common_section_slices.push_back(make_pair(index, go_ahead));
                }
                index += (go_ahead - index);
            }

            if (common_section_slices.size() > 0)
            {
                cout << "\tFound left refactoring production: " << (*it).complete_production() << endl;
            }
            else
            {
                (*it).is_left_refactored = true; // Done refacoring
                continue;
            }

            for (vector<pair<int, int> >::iterator ptr = common_section_slices.begin(); ptr != common_section_slices.end(); ptr++)
            {
                // Find the longest common substring
                int index = 0;
                int small_length = min((*it).productions[(*ptr).first].size(), (*it).productions[(*ptr).second].size());
                while (index < small_length && (*it).productions[(*ptr).first][index] == (*it).productions[(*ptr).second][index])
                    index++;

                string pre = (*it).productions[(*ptr).first].substr(0, index);
                cout << "\t\tFound common substring: " << pre << endl;

                Production variable_prime;
                variable_prime.variable = Production::unused_variable_names[Production::unused_variable_names.size() - 1];
                Production::unused_variable_names.pop_back();
                for (vector<string>::iterator split_prod = (*it).productions.begin() + (*ptr).first; split_prod <= (*it).productions.begin() + (*ptr).second; split_prod++)
                {
                    string new_names;
                    if (index == (*split_prod).size()) //  Fully exhausted
                        new_names = "EPSILON";
                    else
                        new_names = (*split_prod).substr(index, (*split_prod).size());
                    variable_prime.productions.push_back(new_names);

                    // Replace original with variable_prime.variable
                    (*split_prod).replace(index, (*split_prod).size(), variable_prime.variable);
                }

                (*it).productions[(*ptr).first] = pre + variable_prime.variable;
                for (int itr_int = (*ptr).second; itr_int > (*ptr).first; itr_int--)
                {
                    (*it).productions.erase((*it).productions.begin() + itr_int);
                }

                int slice_length = (*ptr).second - (*ptr).first;
                for (vector<pair<int, int> >::iterator leap_ahead = ptr; leap_ahead != common_section_slices.end(); leap_ahead++)
                {
                    (*leap_ahead).first -= slice_length;
                    (*leap_ahead).second -= slice_length;
                }

                cout << "\t\tNew Production created: " << variable_prime.complete_production() << endl;
                cout << "\t\tRefactored Production: " << (*it).complete_production() << endl;
                new_prods.push_back(variable_prime);
            }
            (*it).is_left_refactored = true;
        }
    }

    for (vector<Production>::iterator new_prod = new_prods.begin(); new_prod != new_prods.end(); new_prod++)
    {
        all_productions.push_back((*new_prod));
    }
}

int main()
{
    // Setting up all unused variables
    for (char var = 'A'; var <= 'Z'; var++)
    {
        Production::unused_variable_names.push_back(string(1, var));
    }

    vector<Production> all_productions;
    int ntoken, vtoken;
    ntoken = yylex();
    while (ntoken)
    {
        cout << ntoken << " " << yytext << endl;
        Production new_production;
        new_production.variable = yytext;
        vector<string>::iterator var_location = find(Production::unused_variable_names.begin(), Production::unused_variable_names.end(), new_production.variable);
        Production::unused_variable_names.erase(var_location);
        ntoken = yylex();
        while (ntoken != NEW_LINE)
        {
            if (ntoken == PRODUCTIONS or ntoken == VARIABLE)
            {
                new_production.productions.push_back(yytext);
            }
            ntoken = yylex();
        }
        ntoken = yylex();
        all_productions.push_back(new_production);
    }

    // Printing all Productions
    cout << "\nParsing" << endl;
    for (vector<Production>::iterator it = all_productions.begin(); it != all_productions.end(); ++it)
        cout << "\tProduction: " << (*it).complete_production() << endl;

    // Removing the left recursion
    cout << "\nRemoving left recursion" << endl;
    vector<Production> new_productions;
    for (vector<Production>::iterator it = all_productions.begin(); it != all_productions.end(); ++it)
    {
        bool is_left_recursive = false;
        for (vector<string>::iterator pr = (*it).productions.begin(); pr != (*it).productions.end(); ++pr)
        {
            if ((*it).variable[0] == (*pr)[0])
            {
                is_left_recursive = true;
                break;
            }
        }

        if (is_left_recursive == true)
        {
            cout << "\tFound left recursive production: " << (*it).complete_production() << endl;

            // Separate out left and non left production before splitting
            vector<string> alphas;
            vector<string> betas;
            for (vector<string>::iterator pr = (*it).productions.begin(); pr != (*it).productions.end(); ++pr)
            {
                if ((*it).variable[0] == (*pr)[0]) // left
                {
                    alphas.push_back((*pr).substr(1, (*pr).size()));
                }
                else
                {
                    betas.push_back((*pr));
                }
            }

            if (betas.size() == 0)
            {
                cout << "\tRemoving left recursion fails because there are no non-left productions (betas) for ";
                cout << (*it).complete_production() << endl;
                return 1;
            }

            Production variable_prime;
            variable_prime.variable = Production::unused_variable_names[Production::unused_variable_names.size() - 1];
            Production::unused_variable_names.pop_back();
            // Adjusting left recursion
            (*it).productions.clear();
            for (vector<string>::iterator pr = betas.begin(); pr != betas.end(); ++pr)
            {
                string new_prod = (*pr) + variable_prime.variable;
                (*it).productions.push_back(new_prod);
            }
            // Creating new prime productions
            for (vector<string>::iterator pr = alphas.begin(); pr != alphas.end(); ++pr)
            {
                string new_prod = (*pr) + variable_prime.variable;
                variable_prime.productions.push_back(new_prod);
            }
            variable_prime.productions.push_back("EPSILON");
            new_productions.push_back(variable_prime);
        }
    }
    for (vector<Production>::iterator it = new_productions.begin(); it != new_productions.end(); ++it)
    {
        all_productions.push_back((*it));
    }

    // Printing all Productions
    cout << "\nAfter removing left recursions" << endl;
    for (vector<Production>::iterator it = all_productions.begin(); it != all_productions.end(); ++it)
        cout << "\tProduction: " << (*it).complete_production() << endl;

    // Removing Left Factoring
    cout << "\nRemoving Left Factoring" << endl;
    int batch_factoring = 1;
    while (true)
    {
        cout << "Round " << batch_factoring << " of refactoring -->\n";
        do_left_factoring(all_productions);
        bool all_done = true;
        for (vector<Production>::iterator check_left = all_productions.begin(); check_left != all_productions.end(); check_left++)
        {
            if ((*check_left).is_left_refactored == 0)
            {
                all_done = false;
                break;
            }
        }
        batch_factoring++;
        if (all_done == 1)
        {
            cout << "\tRefactoring done\n";
            break;
        }
    }

    cout << "\nProductions at hand" << endl;
    for (vector<Production>::iterator it = all_productions.begin(); it != all_productions.end(); ++it)
        cout << "\tProduction: " << (*it).complete_production() << endl;
}