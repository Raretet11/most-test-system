<template>
  <div class="task-container">
    <div class="header">
      <div class="header-left">
        <img src="/logo-icon.png" alt="System Logo" class="logo">
        <button class="back-to-archive" @click="$router.push('/')">
          <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <path d="M19 12H5M12 19l-7-7 7-7"/>
          </svg>
          К списку задач
        </button>
      </div>
    </div>

    <div class="main-content-wrapper">
      <div class="main-content">
        <div class="task-content">
          <div class="task-detail">
            <div class="task-header">
              <h1>{{ task.name }}</h1>
              <p class="task-number">Задача #{{ task.id }}</p>
            </div>

            <div class="description">
              <p>{{ task.legend.replace(/\n/g, '<br>') }}</p>
            </div>

            <form @submit.prevent="submitTask" class="submission-form">
              <div ref="editor" class="editor-container"></div>
              <div class="submit-section">
                <button type="submit" :disabled="submitting" class="submit-button">Отправить</button>
                <div class="limits">
                  <span class="limit-text">Time Limit: {{ task.time_limit }} ms</span>
                  <span class="limit-text">Memory Limit: {{ task.memory_limit }} kb</span>
                </div>
              </div>
            </form>
          </div>

          <div class="solutions-list">
            <h2>Ваши попытки</h2>
            <div v-if="loadingSolutions" class="loading">Загрузка...</div>
            <div v-else-if="solutions.length === 0" class="no-solutions">
              Пока попыток не было.
            </div>
            <div v-else>
              <div 
                v-for="solution in reversedSolutions" 
                :key="solution.id"
                class="solution-item"
                :class="{
                  'solution-accepted': solution.verdict === 'Accepted!',
                  'solution-rejected': solution.verdict !== 'Accepted!'
                }"
                @click="showSolutionDetails(solution)"
              >
                <span class="solution-id">#{{ solution.id }}</span>
                <span class="solution-verdict">{{ solution.verdict || 'Pending' }}</span>
                <span class="solution-language">{{ solution.language }}</span>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios';
import { EditorState } from "@codemirror/state";
import { EditorView, keymap } from "@codemirror/view";
import { defaultKeymap } from "@codemirror/commands";
import { python } from "@codemirror/lang-python";
import { basicSetup } from "codemirror";
import { boysAndGirls } from "thememirror";
import { lineNumbers } from '@codemirror/view';

export default {
  name: 'TaskDetail',
  data() {
    return {
      task: {},
      submission: '',
      submitting: false,
      loading: false,
      error: null,
      verdict: '',
      maxTimeLimit: null,
      lastTestInfo: null,
      solutions: [],
      loadingSolutions: false,
      editorView: null,
    };
  },
  computed: {
    reversedSolutions() {
      return [...this.solutions].reverse();
    }
  },
  created() {
    this.fetchTask();
    this.fetchSolutions();
  },
  mounted() {
    this.initEditor();
  },
  methods: {
    initEditor() {
      const startState = EditorState.create({
        doc: '',
        extensions: [
          lineNumbers(),
          python(),
          boysAndGirls,
          keymap.of(defaultKeymap),
          EditorView.updateListener.of((update) => {
            if (update.docChanged) {
              this.submission = update.state.doc.toString();
            }
          }),
        ],
      });

      this.editorView = new EditorView({
        state: startState,
        parent: this.$refs.editor,
      });
    },
    async fetchTask() {
      this.loading = true;
      try {
        const response = await axios.get('/api/tasks/get-by-id/' + this.$route.params.taskNumber);
        this.task = response.data;
      } catch (err) {
        this.error = 'Failed to load task: ' + err.message;
      } finally {
        this.loading = false;
      }
    },
    async fetchSolutions() {
      this.loadingSolutions = true;
      try {
        const response = await axios.get('/api/solutions/get-all', {
          params: {
            task_id: this.$route.params.taskNumber
          }
        });
        this.solutions = response.data;
      } catch (err) {
        console.error('Failed to load solutions:', err);
      } finally {
        this.loadingSolutions = false;
      }
    },
    async submitTask() {
      if (!this.submission.trim()) {
        alert('Submission cannot be empty!');
        return;
      }

      this.submitting = true;
      this.verdict = '';
      this.maxTimeLimit = null;
      this.lastTestInfo = null;

      try {
        const payload = {
          task_id: this.task.id,
          language: 'python',
          solution: this.submission,
        };

        const response = await axios.post('/api/solutions/submit', payload, {
          headers: {
            'Content-Type': 'application/json',
          },
        });

        if (response.status === 200) {
          this.verdict = response.data.verdict || "0";
          this.maxTimeLimit = response.data.max_time_needed || "N/A";
          this.lastTestInfo = response.data.last_test || "0";
          this.submission = '';
          this.editorView.dispatch({
            changes: { from: 0, to: this.editorView.state.doc.length, insert: '' }
          });
          await this.fetchSolutions();
        }
      } catch (err) {
        alert('Failed to submit task: ' + (err.response?.data?.message || err.message));
      } finally {
        this.submitting = false;
      }
    },
    showSolutionDetails(solution) {
      alert(`Solution #${solution.id}\nVerdict: ${solution.verdict}\nLanguage: ${solution.language}`);
    }
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

.back-to-archive {
  display: flex;
  align-items: center;
  gap: 5px;
  padding: 0;
  background: transparent;
  color: #ffffff;
  border: none;
  cursor: pointer;
  font-size: 0.9em;
}

.back-to-archive:hover {
  color: #b3b3b3;
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
}

.task-header {
  padding: 0;
  text-align: left;
}

.task-header h1 {
  color: #ffffff;
  font-size: 1.5em;
  margin-bottom: 5px;
}

.task-number {
  color: #b3b3b3;
  margin-bottom: 0;
  font-size: 0.8em;
}

.task-content {
  display: flex;
  padding: 20px;
  gap: 40px;
  width: 100%;
}

.task-detail {
  flex: 1;
  max-width: calc(100% - 330px);
  color: #ffffff;
  font-family: 'Arial', sans-serif;
  display: flex;
  flex-direction: column;
  gap: 7px; /* Уменьшено расстояние между легендой и полем ввода */
}

.solutions-list {
  width: 260px;
  max-height: calc(100vh - 220px);
  overflow-y: auto;
  font-size: 0.8em;
  padding-right: 10px;
}

.description {
  margin-bottom: 0;
}

.description h2 {
  color: #CFCFCF;
  font-size: 1em;
  margin-bottom: 8px;
}

.description p {
  color: #CFCFCF;
  font-size: 0.9em;
  line-height: 1.5;
}

.submission-form {
  margin-right: 0;
}

.editor-container {
  width: 100%;
  margin-bottom: 20px;
  border: 1px solid #333;
  border-radius: 3px;
  background: #1e1e1e;
  font-size: 0.9em;
}

.submit-section {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.submit-button {
  padding: 8px 20px;
  background: #1e88e5;
  color: #ffffff;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  transition: background 0.2s;
  font-size: 0.8em;
}

.submit-button:hover:not(:disabled) {
  background: #1976d2;
}

.submit-button:disabled {
  background: #64b5f6;
  cursor: not-allowed;
}

.limits {
  display: flex;
  flex-direction: column; /* Установлено вертикальное расположение */
  gap: 5px; /* Уменьшен отступ между элементами */
}

.limit-text {
  color: #757575;
  font-size: 0.7em;
}

.solutions-list h2 {
  color: #ffffff;
  font-size: 1em;
  margin-bottom: 10px;
}

.solution-item {
  padding: 6px 10px;
  margin-bottom: 5px;
  border-radius: 3px;
  cursor: pointer;
  display: flex;
  justify-content: space-between;
  align-items: center;
  background: #1e1e1e;
  border-left: 3px solid #333;
  transition: all 0.2s;
  font-size: 0.75em;
  width: 100%;
  box-sizing: border-box;
  overflow: hidden;
}

.solution-item:hover {
  background: #2a2a2a;
}

.solution-accepted {
  border-left-color: #4caf50;
}

.solution-rejected {
  border-left-color: #ff6b6b;
}

.solution-id {
  color: #b3b3b3;
  flex-shrink: 0;
  min-width: 30px;
}

.solution-verdict {
  font-weight: bold;
  color: #ffffff;
  flex: 1;
  text-align: center;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

.solution-accepted .solution-verdict {
  color: #4caf50;
}

.solution-rejected .solution-verdict {
  color: #ff6b6b;
}

.solution-language {
  color: #909090;
  flex-shrink: 0;
  min-width: 50px;
  text-align: right;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

.loading, .no-solutions {
  color: #b3b3b3;
  text-align: center;
  padding: 10px;
  font-size: 0.8em;
}

button:disabled {
  cursor: not-allowed;
}
</style>