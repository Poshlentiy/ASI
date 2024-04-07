import { Line } from 'react-chartjs-2';
import classes from './ChartWrapper.module.css'
import { Chart as ChartJS } from 'chart.js/auto'

const ChartWrapper = (props) => {

  const labels = [];

  for (let i = 0; i < props.data.length; i++) {
    labels.push('')
  }

  const options = {
    responsive: true,
    plugins: {
      legend: false
    },
    plugins: {
      legend: {
        display: false
      }
    },
    // scales: {
    //   x: {
    //     display: true,
    //   },
    //   y: {
    //     grid: {
    //       color: "black",
    //       borderDash: [20, 4],

    //     },
    //     display: true,
    //   },
    // },
  };

  if (props.time === 'minute') {
    // Настройка параметров для интервала "минута"
    options.scales.y.grid.borderDash = [5, 5];
  } else if (props.time === 'hour') {
    // Настройка параметров для интервала "час"
    options.scales.y.grid.borderDash = [10, 10];
  } else if (props.time === 'day') {
    // Настройка параметров для интервала "день"
    options.scales.y.grid.borderDash = [20, 4];
  } else if (props.time === 'week') {
    // Настройка параметров для интервала "неделя"
    options.scales.y.grid.borderDash = [30, 5];
  }

  const data = {
    labels,
    datasets: [
      {
        label: 'Dataset 1',
        data: props.data,
        borderColor: props.color,
        backgroundColor: props.color,
      }
    ],
  };

  return (
    <div className={classes.container}>
      <div className={classes.label}>{props.text}</div>
      <Line options={options} data={data} />
    </div>
  )
}

export default ChartWrapper