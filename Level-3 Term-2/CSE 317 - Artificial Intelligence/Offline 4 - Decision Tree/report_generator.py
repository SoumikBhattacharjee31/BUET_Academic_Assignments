import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix
from reportlab.lib.pagesizes import letter
from reportlab.pdfgen import canvas
from reportlab.platypus import Table, TableStyle
from reportlab.lib import colors

# Load CSV data
df = pd.read_csv('log.csv')

# Preprocess Data
df['traintime'] = pd.to_numeric(df['traintime'], errors='coerce')
df['testtime'] = pd.to_numeric(df['testtime'], errors='coerce')
df['truevalue'] = df['truevalue'].astype(str)
df['predictedvalue'] = df['predictedvalue'].astype(str)

# Accuracy Bar Plot (Group Gini and Information Gain)
accuracies_grouped = df.groupby(['heuristic', 'bestortop3']).apply(
    lambda group: accuracy_score(group['truevalue'], group['predictedvalue'])
).reset_index(name='accuracy')

plt.figure(figsize=(10, 6))
sns.barplot(x='bestortop3', y='accuracy', hue='heuristic', data=accuracies_grouped)
plt.title('Accuracy Comparison Grouped by Gini and Information Gain')
plt.ylabel('Accuracy')
plt.xlabel('Method (Best or Top 3)')
plt.xticks(rotation=45)

# Annotate bars with accuracy values
for bar in plt.gca().patches:
    plt.gca().annotate(
        f'{bar.get_height():.2f}', 
        (bar.get_x() + bar.get_width() / 2, bar.get_height()), 
        ha='center', va='bottom', fontsize=8
    )
plt.tight_layout()
accuracy_image_path_grouped = 'accuracy_comparison_grouped.png'
plt.savefig(accuracy_image_path_grouped)
plt.close()

# Training Time Box Plot (Normalized)
df['normalized_traintime'] = df['traintime'] / df['traintime'].max()
plt.figure(figsize=(10, 6))
sns.boxplot(x='heuristic', y='normalized_traintime', hue='bestortop3', data=df)
plt.title('Training Time Comparison (Normalized)')
plt.ylabel('Normalized Training Time')
plt.xlabel('Heuristic')
plt.xticks(rotation=45)
plt.tight_layout()
training_time_image_path = 'training_time_comparison_normalized.png'
plt.savefig(training_time_image_path)
plt.close()

# Accuracy by Index for 'Best'
best_accuracies = df[df['bestortop3'] == 'best'].groupby(['heuristic', 'index'])[['truevalue', 'predictedvalue']].apply(
    lambda x: accuracy_score(x['truevalue'], x['predictedvalue'])
).reset_index(name='accuracy')

plt.figure(figsize=(12, 6))
sns.barplot(x='index', y='accuracy', hue='heuristic', data=best_accuracies)
plt.title('Accuracy Comparison by Index (Best)')
plt.ylabel('Accuracy')
plt.xlabel('Index')
plt.xticks(rotation=45)

# Annotate bars with accuracy values
for bar in plt.gca().patches:
    plt.gca().annotate(
        f'{bar.get_height():.2f}',
        (bar.get_x() + bar.get_width() / 2, bar.get_height()),
        ha='center', va='bottom', fontsize=8
    )
plt.tight_layout()
best_accuracy_image_path = 'accuracy_by_index_best.png'
plt.savefig(best_accuracy_image_path)
plt.close()

# Accuracy by Index for 'Top 3'
top3_accuracies = df[df['bestortop3'] == 'top3'].groupby(['heuristic', 'index'])[['truevalue', 'predictedvalue']].apply(
    lambda x: accuracy_score(x['truevalue'], x['predictedvalue'])
).reset_index(name='accuracy')

plt.figure(figsize=(12, 6))
sns.barplot(x='index', y='accuracy', hue='heuristic', data=top3_accuracies)
plt.title('Accuracy Comparison by Index (Top 3)')
plt.ylabel('Accuracy')
plt.xlabel('Index')
plt.xticks(rotation=45)

# Annotate bars with accuracy values
for bar in plt.gca().patches:
    plt.gca().annotate(
        f'{bar.get_height():.2f}',
        (bar.get_x() + bar.get_width() / 2, bar.get_height()),
        ha='center', va='bottom', fontsize=8
    )
plt.tight_layout()
top3_accuracy_image_path = 'accuracy_by_index_top3.png'
plt.savefig(top3_accuracy_image_path)
plt.close()


# Import necessary libraries
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.metrics import confusion_matrix, classification_report

# Generate Confusion Matrix Heatmaps and Classification Reports for 'Best' with heuristic 'gini'
best_df_gini = df[(df['bestortop3'] == 'best') & (df['heuristic'] == 'gini')]
best_conf_matrix_gini = confusion_matrix(
    best_df_gini['truevalue'], best_df_gini['predictedvalue'], labels=df['truevalue'].unique()
)
plt.figure(figsize=(8, 6))
sns.heatmap(best_conf_matrix_gini, annot=True, fmt='d', cmap='Blues', 
            xticklabels=df['truevalue'].unique(), yticklabels=df['truevalue'].unique())
plt.title('Confusion Matrix (Best, Gini)')
plt.ylabel('True Label')
plt.xlabel('Predicted Label')
plt.tight_layout()
best_conf_matrix_image_path_gini = 'confusion_matrix_best_gini.png'
plt.savefig(best_conf_matrix_image_path_gini)
plt.close()

best_class_report_gini = classification_report(
    best_df_gini['truevalue'], best_df_gini['predictedvalue'], output_dict=True
)
best_class_report_df_gini = pd.DataFrame(best_class_report_gini).transpose()

# Generate Confusion Matrix Heatmaps and Classification Reports for 'Best' with heuristic 'information_gain'
best_df_information_gain = df[(df['bestortop3'] == 'best') & (df['heuristic'] != 'gini')]
best_conf_matrix_information_gain = confusion_matrix(
    best_df_information_gain['truevalue'], best_df_information_gain['predictedvalue'], labels=df['truevalue'].unique()
)
plt.figure(figsize=(8, 6))
sns.heatmap(best_conf_matrix_information_gain, annot=True, fmt='d', cmap='Blues', 
            xticklabels=df['truevalue'].unique(), yticklabels=df['truevalue'].unique())
plt.title('Confusion Matrix (Best, Information Gain)')
plt.ylabel('True Label')
plt.xlabel('Predicted Label')
plt.tight_layout()
best_conf_matrix_image_path_information_gain = 'confusion_matrix_best_information_gain.png'
plt.savefig(best_conf_matrix_image_path_information_gain)
plt.close()

best_class_report_information_gain = classification_report(
    best_df_information_gain['truevalue'], best_df_information_gain['predictedvalue'], output_dict=True
)
best_class_report_df_information_gain = pd.DataFrame(best_class_report_information_gain).transpose()

# Generate Confusion Matrix Heatmaps and Classification Reports for 'Top 3' with heuristic 'gini'
top3_df_gini = df[(df['bestortop3'] == 'top3') & (df['heuristic'] == 'gini')]
top3_conf_matrix_gini = confusion_matrix(
    top3_df_gini['truevalue'], top3_df_gini['predictedvalue'], labels=df['truevalue'].unique()
)
plt.figure(figsize=(8, 6))
sns.heatmap(top3_conf_matrix_gini, annot=True, fmt='d', cmap='Blues', 
            xticklabels=df['truevalue'].unique(), yticklabels=df['truevalue'].unique())
plt.title('Confusion Matrix (Top 3, Gini)')
plt.ylabel('True Label')
plt.xlabel('Predicted Label')
plt.tight_layout()
top3_conf_matrix_image_path_gini = 'confusion_matrix_top3_gini.png'
plt.savefig(top3_conf_matrix_image_path_gini)
plt.close()

top3_class_report_gini = classification_report(
    top3_df_gini['truevalue'], top3_df_gini['predictedvalue'], output_dict=True
)
top3_class_report_df_gini = pd.DataFrame(top3_class_report_gini).transpose()

# Generate Confusion Matrix Heatmaps and Classification Reports for 'Top 3' with heuristic 'information_gain'
top3_df_information_gain = df[(df['bestortop3'] == 'top3') & (df['heuristic'] != 'gini')]
top3_conf_matrix_information_gain = confusion_matrix(
    top3_df_information_gain['truevalue'], top3_df_information_gain['predictedvalue'], labels=df['truevalue'].unique()
)
plt.figure(figsize=(8, 6))
sns.heatmap(top3_conf_matrix_information_gain, annot=True, fmt='d', cmap='Blues', 
            xticklabels=df['truevalue'].unique(), yticklabels=df['truevalue'].unique())
plt.title('Confusion Matrix (Top 3, Information Gain)')
plt.ylabel('True Label')
plt.xlabel('Predicted Label')
plt.tight_layout()
top3_conf_matrix_image_path_information_gain = 'confusion_matrix_top3_information_gain.png'
plt.savefig(top3_conf_matrix_image_path_information_gain)
plt.close()

top3_class_report_information_gain = classification_report(
    top3_df_information_gain['truevalue'], top3_df_information_gain['predictedvalue'], output_dict=True
)
top3_class_report_df_information_gain = pd.DataFrame(top3_class_report_information_gain).transpose()


from reportlab.lib.pagesizes import letter
from reportlab.platypus import Table, TableStyle
from reportlab.lib import colors
from reportlab.pdfgen import canvas

# Updated PDF Report Generation
def create_pdf_report(
    accuracy_image,
    training_image,
    best_gini_conf_matrix_image,
    best_information_gain_conf_matrix_image,
    top3_gini_conf_matrix_image,
    top3_information_gain_conf_matrix_image,
    best_accuracy_image,
    top3_accuracy_image,
    best_gini_class_report_df,
    best_information_gain_class_report_df,
    top3_gini_class_report_df,
    top3_information_gain_class_report_df
):
    pdf_path = 'decision_tree_performance_report.pdf'
    c = canvas.Canvas(pdf_path, pagesize=letter)
    width, height = letter

    # Title Page
    c.setFont("Helvetica-Bold", 16)
    c.drawString(50, 750, "Decision Tree Performance Report")
    c.setFont("Helvetica", 12)
    c.drawString(50, 730, "This report evaluates the performance of decision tree models.")
    c.showPage()

    # Accuracy Graph
    c.setFont("Helvetica-Bold", 14)
    c.drawString(50, 750, "Accuracy Comparison")
    c.drawImage(accuracy_image, 50, 400, width=500, height=300)
    c.showPage()

    # Training Time Graph
    c.setFont("Helvetica-Bold", 14)
    c.drawString(50, 750, "Training Time Comparison")
    c.drawImage(training_image, 50, 400, width=500, height=300)
    c.showPage()

    # Accuracy by Index (Best)
    c.setFont("Helvetica-Bold", 14)
    c.drawString(50, 750, "Accuracy Comparison by Index (Best)")
    c.drawImage(best_accuracy_image, 50, 400, width=500, height=300)
    c.showPage()

    # Accuracy by Index (Top 3)
    c.setFont("Helvetica-Bold", 14)
    c.drawString(50, 750, "Accuracy Comparison by Index (Top 3)")
    c.drawImage(top3_accuracy_image, 50, 400, width=500, height=300)
    c.showPage()

    # Confusion Matrix (Best, Gini)
    c.setFont("Helvetica-Bold", 14)
    c.drawString(50, 750, "Confusion Matrix (Best, Gini)")
    c.drawImage(best_gini_conf_matrix_image, 50, 400, width=500, height=300)
    c.showPage()

    # Confusion Matrix (Best, Information Gain)
    c.setFont("Helvetica-Bold", 14)
    c.drawString(50, 750, "Confusion Matrix (Best, Information Gain)")
    c.drawImage(best_information_gain_conf_matrix_image, 50, 400, width=500, height=300)
    c.showPage()

    # Confusion Matrix (Top 3, Gini)
    c.setFont("Helvetica-Bold", 14)
    c.drawString(50, 750, "Confusion Matrix (Top 3, Gini)")
    c.drawImage(top3_gini_conf_matrix_image, 50, 400, width=500, height=300)
    c.showPage()

    # Confusion Matrix (Top 3, Information Gain)
    c.setFont("Helvetica-Bold", 14)
    c.drawString(50, 750, "Confusion Matrix (Top 3, Information Gain)")
    c.drawImage(top3_information_gain_conf_matrix_image, 50, 400, width=500, height=300)
    c.showPage()

    # Classification Report (Best, Gini)
    c.setFont("Helvetica-Bold", 14)
    c.drawString(50, 750, "Classification Report (Best, Gini)")
    create_classification_table(c, best_gini_class_report_df, width, height)
    c.showPage()

    # Classification Report (Best, Information Gain)
    c.setFont("Helvetica-Bold", 14)
    c.drawString(50, 750, "Classification Report (Best, Information Gain)")
    create_classification_table(c, best_information_gain_class_report_df, width, height)
    c.showPage()

    # Classification Report (Top 3, Gini)
    c.setFont("Helvetica-Bold", 14)
    c.drawString(50, 750, "Classification Report (Top 3, Gini)")
    create_classification_table(c, top3_gini_class_report_df, width, height)
    c.showPage()

    # Classification Report (Top 3, Information Gain)
    c.setFont("Helvetica-Bold", 14)
    c.drawString(50, 750, "Classification Report (Top 3, Information Gain)")
    create_classification_table(c, top3_information_gain_class_report_df, width, height)
    c.showPage()

    # Save PDF
    c.save()
    print(f"PDF Report saved at: {pdf_path}")


def create_classification_table(c, class_report_df, width, height):
    """Helper function to draw classification report table."""
    class_report_df = class_report_df.applymap(lambda x: f"{x:.4f}" if isinstance(x, float) else x)
    table_data = [list(class_report_df.columns)] + class_report_df.reset_index().values.tolist()
    table_data[0].insert(0, "")
    table = Table(table_data, colWidths=[100, 80, 80, 80, 80])
    table.setStyle(TableStyle([
        ('BACKGROUND', (0, 0), (-1, 0), colors.grey),
        ('TEXTCOLOR', (0, 0), (-1, 0), colors.whitesmoke),
        ('ALIGN', (0, 0), (-1, -1), 'CENTER'),
        ('FONTNAME', (0, 0), (-1, 0), 'Helvetica-Bold'),
        ('FONTSIZE', (0, 0), (-1, -1), 10),
        ('BOTTOMPADDING', (0, 0), (-1, 0), 12),
        ('BACKGROUND', (0, 1), (-1, -1), colors.beige),
        ('GRID', (0, 0), (-1, -1), 1, colors.black),
    ]))
    table.wrapOn(c, width, height)
    table.drawOn(c, 50, 450)


# Call the updated PDF report generation function
create_pdf_report(
    accuracy_image_path_grouped,                      # Path to accuracy comparison graph
    training_time_image_path,                         # Path to training time comparison graph
    best_conf_matrix_image_path_gini,                 # Path to confusion matrix (Best, Gini)
    best_conf_matrix_image_path_information_gain,     # Path to confusion matrix (Best, Information Gain)
    top3_conf_matrix_image_path_gini,                 # Path to confusion matrix (Top 3, Gini)
    top3_conf_matrix_image_path_information_gain,     # Path to confusion matrix (Top 3, Information Gain)
    best_accuracy_image_path,                         # Path to accuracy comparison graph for Best
    top3_accuracy_image_path,                         # Path to accuracy comparison graph for Top 3
    best_class_report_df_gini,                        # Classification report DataFrame (Best, Gini)
    best_class_report_df_information_gain,            # Classification report DataFrame (Best, Information Gain)
    top3_class_report_df_gini,                        # Classification report DataFrame (Top 3, Gini)
    top3_class_report_df_information_gain             # Classification report DataFrame (Top 3, Information Gain)
)
