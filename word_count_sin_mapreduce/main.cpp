#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <string>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <chrono>

int num_threads = 6;
int threads_completed = 0;
std::vector<std::unordered_map<std::string, int>> vec_words_counted(num_threads);
std::ifstream newfile;

std::mutex mtx_newfile;
std::mutex mtx_current_line;
std::mutex mtx_threads_completed;

int num_lines_to_process = 60000;
//int num_lines_to_process = 10000;

void process_word_count(int id)
{
    std::string line;
    std::vector<std::string> lines_to_process;

    while (true)
    {
        lines_to_process = std::vector<std::string>();

        mtx_newfile.lock();
        int cont = 0;
        while (cont < num_lines_to_process && std::getline(newfile, line)){
            lines_to_process.push_back(line);
            ++cont;
        }
        mtx_newfile.unlock();
        for (int i = 0; i < lines_to_process.size(); ++i)
        {
            std::istringstream iss(lines_to_process[i]);
            std::string s;
            while (std::getline(iss, s, ' ')) {
                if (vec_words_counted[id].find(s) == vec_words_counted[id].end())
                {
                    vec_words_counted[id][s] = 1;
                }
                else
                {
                    vec_words_counted[id][s] += 1;
                }
            }
        }
        if (cont != num_lines_to_process)
        {
            mtx_threads_completed.lock();
            threads_completed += 1;
            mtx_threads_completed.unlock();
            return;
        }
    }
}

int main()
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    newfile.open("archivo_grande.txt", std::ios::in);
    //newfile.open("archivo_prueba.txt", std::ios::in);

    for (int i = 0; i < num_threads; ++i)
    {
        std::thread(process_word_count, i).detach();
    }

    while (true)
    {
        mtx_threads_completed.lock();
        if (threads_completed == num_threads)
        {
            mtx_threads_completed.unlock();
            break;
        }
        mtx_threads_completed.unlock();
    }
    newfile.close();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[seconds]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::minutes>(end - begin).count() << "[minutes]" << std::endl;

    std::string word1 = "the";
    int count1 = 0;
    for (int i = 0; i < num_threads; ++i)
    {
        count1 += vec_words_counted[i][word1];
    }
    std::cout << count1 << "\n";

    return 0;
}
