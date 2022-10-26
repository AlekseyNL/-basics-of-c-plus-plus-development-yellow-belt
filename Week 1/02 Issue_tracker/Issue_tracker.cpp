
#include <iostream>
#include <map>
#include <tuple>
#include <vector>


using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks
{
private:
    // Хранит количество задач каждого статуса для каждого разработчика в команде
    map <string, TasksInfo> m_table_teamTasks;

public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const
    {      
        return (m_table_teamTasks.find(person)->second );
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person)
    {
        TasksInfo tasks = m_table_teamTasks[person];
        int number_of_tasks_in_NEW = tasks[TaskStatus::NEW];
        tasks[TaskStatus::NEW] = ++number_of_tasks_in_NEW;
        m_table_teamTasks[person] = tasks;
    }


    // Обновить статусы задач конкретного разработчика
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count)
    {
        TasksInfo old_TasksInfo, new_TasksInfo, person_TasksInfo;

        if (m_table_teamTasks.count(person))
        {
            person_TasksInfo = m_table_teamTasks[person];
        
            vector<int> new_values(4);
            vector<int> old_values(4);

            for (size_t i = 0; i <= static_cast<int>(TaskStatus::DONE);
                ++i)
            {
                if (person_TasksInfo.count(static_cast<TaskStatus>(i)))
                {
                    old_values[i] = person_TasksInfo[static_cast<TaskStatus>(i)];
                }
            }

            int count = task_count;
            for (size_t i = 0; i  < static_cast<int>(TaskStatus::DONE); ++i)
            {   
                if (task_count == 0 || count == 0)
                {                    
                    break;
                }   

                if (old_values[i] > count)
                {
                    new_values[i + 1] = count;
                    old_values[i] -= count;
                    count = 0;
                }
                if (old_values[i] <= count && old_values[i] > 0)
                {
                    new_values[i + 1] = old_values[i];
                    count -= old_values[i];
                    old_values[i] = 0;
                }
            }

            new_TasksInfo.clear();
            old_TasksInfo.clear();
            person_TasksInfo.clear();
            for (int i = 0; i <= static_cast<int>(TaskStatus::DONE); ++i)
            {
                if (old_values[i] || new_values[i])
                {
                    person_TasksInfo[static_cast<TaskStatus>(i)] = old_values[i] + new_values[i];

                    if (new_values[i])
                    {
                        new_TasksInfo[static_cast<TaskStatus>(i)] = new_values[i];
                    }
                    if (old_values[i] && i != static_cast<int>(TaskStatus::DONE))
                    {
                        old_TasksInfo[static_cast<TaskStatus>(i)] = old_values[i];
                    }
                }
            }
            m_table_teamTasks[person] = person_TasksInfo;
        }
        return tie(new_TasksInfo, old_TasksInfo);
    }
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [ ] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
        ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
        ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
        ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main()
{
    TeamTasks tasks;
   
    tasks.AddNewTask("Ilia");
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) =
        tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
        tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);
  
    cout << endl;

    cout << "__________________________________" << endl;
    tie(updated_tasks, untouched_tasks) =
        tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
        tasks.PerformPersonTasks("Ivan", 1);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
        tasks.PerformPersonTasks("Ivan", 1);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
        tasks.PerformPersonTasks("Ivan", 1);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
        tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    return 0;
}
