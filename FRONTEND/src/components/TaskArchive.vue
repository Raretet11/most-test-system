<template>
  <div class="task-container">
    <div class="header">
      <div class="header-left">
        <img src="/logo-icon.png" alt="System Logo" class="logo">
      </div>
    </div>

    <div class="main-content-wrapper">
      <div class="main-content">
        <h1>MOST test sys.</h1>
        <div v-if="loading" class="loading">Loading tasks...</div>
        <div v-else-if="error" class="error-message">{{ error }}</div>
        <div v-else class="task-list">
          <button
            v-for="task in tasks"
            :key="task.id"
            class="task-button"
            @click="$router.push({ name: 'TaskDetail', params: { taskNumber: task.id } })"
          >
            <div class="task-id">{{ task.id }}</div>
            <div class="task-content">
              <h2>{{ task.name }}</h2>
              <p>{{ task.description }}</p>
            </div>
          </button>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios';

export default {
  name: 'TaskArchive',
  data() {
    return {
      tasks: [],
      loading: false,
      error: null,
    };
  },
  created() {
    this.fetchTasks();
  },
  methods: {
    async fetchTasks() {
      this.loading = true;
      try {
        const response = await axios.get('/api/tasks/get-all');
        this.tasks = response.data;
      } catch (err) {
        this.error = 'Failed to load tasks: ' + err.message;
      } finally {
        this.loading = false;
      }
    },
  },
};
</script>

<style scoped>
/* Подключение шрифта Roboto через Google Fonts */
@import url('https://fonts.googleapis.com/css2?family=Gidole&display=swap');

* {
  font-family: 'Gidole', sans-serif;
}

.task-container {
  max-width: 900px;
  margin: 0 auto;
  padding: 15px;
}

.header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 15px;
}

.header-left {
  display: flex;
  align-items: center;
  gap: 15px;
}

.logo {
  height: 50px;
  width: 50px;
}

.main-content-wrapper {
  border: 1px solid #333;
  border-radius: 6px;
  overflow: hidden;
  width: 100%;
}

.main-content {
  background: #121212;
  display: flex;
  flex-direction: column;
  padding: 20px;
}

h1 {
  color: #ffffff;
  text-align: center;
  margin-bottom: 20px;
  font-size: 1.5em;
}

.loading {
  color: #b3b3b3;
  text-align: center;
  padding: 10px;
  font-size: 0.9em;
}

.error-message {
  color: #ff6b6b;
  text-align: center;
  padding: 10px;
  background: #1e1e1e;
  border: 1px solid #ff6b6b;
  border-radius: 4px;
  font-size: 0.9em;
}

.task-list {
  display: flex;
  flex-direction: column;
  gap: 10px;
}

.task-button {
  background: #1e1e1e;
  border: 1px solid #333;
  border-radius: 4px;
  padding: 12px 15px;
  text-align: left;
  color: #ffffff;
  cursor: pointer;
  transition: background 0.2s;
  display: flex;
  align-items: center;
  gap: 15px;
  width: 100%;
}

.task-button:hover {
  background: #2a2a2a;
}

.task-id {
  background: #333;
  color: #ffffff;
  padding: 5px 10px;
  border-radius: 3px;
  font-size: 0.8em;
  min-width: 30px;
  text-align: center;
}

.task-content h2 {
  margin: 0 0 5px 0;
  font-size: 1em;
  color: #ffffff;
}

.task-content p {
  margin: 0;
  color: #b3b3b3;
  font-size: 0.8em;
}
</style>