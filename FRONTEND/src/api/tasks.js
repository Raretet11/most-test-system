export async function getTasks() {
    const response = await fetch('api/tasks/get-all');
    if (!response.ok) {
      throw new Error('Ошибка при получении задач');
    }
    const data = await response.json();
    return data;
  }