import csv
from tabulate import tabulate

def calculate_error_percentage(actual, computed):
    if actual == 0:
        return 0  # Avoid division by zero
    return abs((computed - actual) / actual) * 100

def process_file(file_path):
    data = {}
    true_result = [2579, 7542, 118282, 6110, 6528, 426, 538, 629, 21282, 22141, 20749, 21294, 22068, 14379, 42029, 108159, 59030, 58537, 1211, 2323, 675]

    with open(file_path, 'r') as file:
        reader = csv.DictReader(file)
        for row in reader:
            comment = row['Comment'].split('-')
            computed_distance = float(row['Total Distance'])
            heuristic = int(comment[1]) * 16 + int(comment[2])
            actual_distance = true_result[int(comment[0])]
            runtime = float(row['Runtime (microseconds)']) / 1_000_000  # Convert microseconds to seconds
            
            # Calculate error
            error = calculate_error_percentage(actual_distance, computed_distance)
            
            # Initialize data structure if heuristic is encountered for the first time
            if heuristic not in data:
                data[heuristic] = {
                    'errors': [],
                    'runtimes': [],
                }
            
            # Append the error and runtime to the respective heuristic
            data[heuristic]['errors'].append(error)
            data[heuristic]['runtimes'].append(runtime)
    
    # Prepare summary
    summary = []
    for heuristic, values in data.items():
        errors = values['errors']
        runtimes = values['runtimes']

        if heuristic // 16 == 0:
            constructive = "Furthest"
        elif heuristic // 16 == 1:
            constructive = "Cheapest"
        elif heuristic // 16 == 2:
            constructive = "Nearest"
        
        perturbative_methods = [
            "", "2opt", "swap", "shift", "shift+2opt", "2opt+shift", 
            "2opt+swap", "swap+2opt", "swap+shift", "shift+swap", 
            "shift+2opt+swap", "2opt+shift+swap", "2opt+swap+shift", 
            "swap+2opt+shift", "swap+shift+2opt", "shift+swap+2opt"
        ]
        
        perturbative = perturbative_methods[heuristic % 16]
        
        avg_error = sum(errors) / len(errors)
        avg_runtime = sum(runtimes) / len(runtimes)

        summary.append({
            'Constructive': constructive,
            'Perturbative': perturbative,
            'Avg Error (%)': avg_error,
            'Max Error (%)': max(errors),
            'Min Error (%)': min(errors),
            'Avg Runtime (s)': avg_runtime,
            'Max Runtime (s)': max(runtimes),
            'Min Runtime (s)': min(runtimes)
        })
    
    # Write summary in tabular format
    with open("summary.txt", "w") as f:
        f.write(tabulate(summary, headers='keys', tablefmt='grid'))
    
    # Find the best performing heuristic based on lowest Avg Error and lowest Avg Runtime
    best_by_error = min(summary, key=lambda x: x['Avg Error (%)'])
    best_by_error2 = min([x for x in summary if len(x["Perturbative"])<15], key=lambda x: x['Avg Error (%)'])
    best_by_error3 = min([x for x in summary if "+" not in x["Perturbative"]], key=lambda x: x['Avg Error (%)'])

    print("Best Heuristic with three perturbative method:")
    print(f"Constructive: {best_by_error['Constructive']}, Perturbative: {best_by_error['Perturbative']}")
    print(f"Average Error: {best_by_error['Avg Error (%)']:.2f}%")
    print(f"Average Runtime: {best_by_error['Avg Runtime (s)']:.6f}s\n")
    
    print("Best Heuristic with two perturbative method:")
    print(f"Constructive: {best_by_error2['Constructive']}, Perturbative: {best_by_error2['Perturbative']}")
    print(f"Average Error: {best_by_error2['Avg Error (%)']:.2f}%")
    print(f"Average Runtime: {best_by_error2['Avg Runtime (s)']:.6f}s\n")
    
    print("Best Heuristic with one perturbative method:")
    print(f"Constructive: {best_by_error3['Constructive']}, Perturbative: {best_by_error3['Perturbative']}")
    print(f"Average Error: {best_by_error3['Avg Error (%)']:.2f}%")
    print(f"Average Runtime: {best_by_error3['Avg Runtime (s)']:.6f}s\n")

# Usage Example
file_path = 'log.txt'  # Replace with your file path
process_file(file_path)
