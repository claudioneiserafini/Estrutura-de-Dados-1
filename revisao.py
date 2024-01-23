def task_manager():
    tasks = []

    while True:
        # Exibir menu em uma caixa
        print("\n|" + "-" * 34 +"|")
        print("|" + "Menu:".center(34) + "|")
        print("|" + "-" * 34 + "|")
        print("|" + " 1. Adicionar nova tarefa".ljust(34) + "|")
        print("|" + " 2. Listar todas as tarefas".ljust(34) + "|")
        print("|" + " 3. Remover tarefa".ljust(34) + "|")
        print("|" + " 4. Sair".ljust(34) + "|")
        print("|" + "-" * 34 +"|")

        # Obter escolha do usuário
        try:
            choice = int(input("| Escolha uma opção (1-4): "))
        except ValueError:
            print("Por favor, insira um número válido.")
            continue

        if choice == 1:
            # Adicionar nova tarefa
            task = input("| Insira o nome da tarefa: ")
            tasks.append(task)
            print(f"| Tarefa '{task}' adicionada com sucesso!")
        elif choice == 2:
            # Listar todas as tarefas
            if tasks:
                print("|\n| Tarefas:")
                for idx, task in enumerate(tasks):
                    print(f"| {idx + 1}. {task}")
            else:
                print("| Nenhuma tarefa encontrada.")
        elif choice == 3:
            # Remover tarefa
            if tasks:
                try:
                    task_number = int(input("| Insira o número da tarefa para remover: ")) - 1
                    if 0 <= task_number < len(tasks):
                        removed_task = tasks.pop(task_number)
                        print(f"| Tarefa '{removed_task}' removida com sucesso!")
                    else:
                        print("| Número da tarefa inválido.")
                except ValueError:
                    print("| Por favor, insira um número válido.")
            else:
                print("| Não há tarefas para remover.")
        elif choice == 4:
            # Sair
            print("| Saindo do programa...")
            break
        else:
            print("| Opção inválida. Por favor, escolha uma opção de 1 a 4.")

# Executar o gerenciador de tarefas
task_manager()
