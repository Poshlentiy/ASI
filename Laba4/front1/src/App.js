import { useState } from 'react';
import classes from './App.module.css';
import Values from './pages/Values';
import Graphics from './pages/Graphics';

function App() {

  const [page, setPage] = useState('values')

  return (
    <div className={classes.container}>
      {<div className={classes.buttons}>
        <button onClick={() => setPage('values')}>{`Параметры`}</button>
        <button onClick={() => setPage('graphic')}>{`Вывод графиков`}</button>
      </div>}
      <div className={classes.page}>
        <div>
          <Values/>
        </div>
        
        <Graphics/>
      </div>
    </div>
  );
}

export default App;
