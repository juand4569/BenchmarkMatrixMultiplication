import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

results_dir = Path("results")
df_java = pd.read_csv(results_dir / "benchmark.csv")
df_c = pd.read_csv(results_dir / "benchmark_c.csv")
df_python = pd.read_csv(results_dir / "matrix_benchmark.csv")

df = pd.concat([df_c, df_python, df_java], ignore_index=True)

fig1, ax1 = plt.subplots(figsize=(8, 6))
for lang in ['C', 'Python', 'Java']:
    data = df[df['Language'] == lang].sort_values('Matrix_Size')
    ax1.plot(data['Matrix_Size'], data['Time_ms'], marker='o', label=lang)
ax1.set_xlabel('Matrix Size')
ax1.set_ylabel('Time (ms)')
ax1.set_title('Execution Time (Log Scale)')
ax1.set_xscale('log', base=2)
ax1.set_yscale('log')
ax1.legend()
ax1.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig(results_dir / "time_log.png", dpi=150)
plt.close()

fig2, ax2 = plt.subplots(figsize=(8, 6))
for lang in ['C', 'Python', 'Java']:
    data = df[df['Language'] == lang].sort_values('Matrix_Size')
    ax2.plot(data['Matrix_Size'], data['Memory_MB'], marker='o', label=lang)
ax2.set_xlabel('Matrix Size')
ax2.set_ylabel('Memory (MB)')
ax2.set_title('Memory Usage')
ax2.set_xscale('log', base=2)
ax2.legend()
ax2.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig(results_dir / "memory.png", dpi=150)
plt.close()

fig3, ax3 = plt.subplots(figsize=(8, 6))
for lang in ['C', 'Python', 'Java']:
    data = df[df['Language'] == lang].sort_values('Matrix_Size')
    ax3.plot(data['Matrix_Size'], data['Time_ms'], marker='o', label=lang)
ax3.set_xlabel('Matrix Size')
ax3.set_ylabel('Time (ms)')
ax3.set_title('Execution Time (Linear Scale)')
ax3.set_xscale('log', base=2)
ax3.legend()
ax3.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig(results_dir / "time_linear.png", dpi=150)
plt.close()

print(f"✅ Plots saved:")
print(f"   - {results_dir / 'time_log.png'}")
print(f"   - {results_dir / 'memory.png'}")
print(f"   - {results_dir / 'time_linear.png'}")

print("\n" + df.sort_values(['Language', 'Matrix_Size']).to_string(index=False))