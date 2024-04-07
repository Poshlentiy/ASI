import ChartWrapper from "../Components/ChartWrapper/ChartWrapper"
import classes from './Graphics.module.css'
import { useEffect, useState } from "react"
import axios from "axios"


const Graphics = () => {

    const _MINUTE = 'minute', _HOUR = 'hour', _DAY = 'day', _WEEK = 'week'

    const [time, setTime] = useState(_MINUTE)

    const [cpuTemp, setCpuTemp] = useState([])
    const [gpuTemp, setGpuTemp] = useState([])
    const [cpuLoad, setCpuLoad] = useState([])
    const [gpuLoad, setGpuLoad] = useState([])
    const [ramTemp, setRamTemp] = useState([])

    const fetchData = async () => {

        let step;
        switch (time) {
            case _MINUTE:
                step = 1; // Шаг для интервала в минуту
                break;
            case _HOUR:
                step = 60; // Шаг для интервала в час
                break;
            case _DAY:
                step = 1440; // Шаг для интервала в день (24 часа * 60 минут)
                break;
            case _WEEK:
                step = 10080; // Шаг для интервала в неделю (7 дней * 24 часа * 60 минут)
                break;
            default:
                step = 1; // По умолчанию, если не найден соответствующий интервал
        }

        const { data } = await axios.post('http://192.168.116.54:8000/time_interval_values', { interaval: time, step: step });


        console.log(data);

        const cpuTempArray = []
        const gpuTempArray = []
        const cpuLoadArray = []
        const gpuLoadArray = []
        const ramTempArray = []

        for (let i = 0; i < data.length; i++) {
            cpuTempArray.push(data[i].cpu_temp)
            gpuTempArray.push(data[i].gpu_temp)
            cpuLoadArray.push(data[i].cpu_busy)
            gpuLoadArray.push(data[i].gpu_busy)
            ramTempArray.push(data[i].ram_busy)
        }

        setCpuTemp(cpuTempArray)
        setGpuTemp(gpuTempArray)
        setCpuLoad(cpuLoadArray)
        setGpuLoad(gpuLoadArray)
        setRamTemp(ramTempArray)
    }

    useEffect(() => {
        const interval = setInterval(() => {
            fetchData();
        }, 1000); // вызывать функцию каждую секунду

        return () => {
            clearInterval(interval); // очистить интервал при размонтировании компонента
        };
    }, [time]);

    return (
        <div className={classes.container}>
            <header style={{ fontSize: '26px', fontWeight: 'bold', margin: '0 auto', marginBottom: '20px', textAlign: 'center' }}>Changing Curve</header>
            <div className={classes.time_buttons}>
                <button onClick={() => setTime(_MINUTE)}>
                    Minute
                </button>
                <button onClick={() => setTime(_HOUR)}>
                    Hour
                </button>
                <button onClick={() => setTime(_DAY)}>
                    Day
                </button>
                <button onClick={() => setTime(_WEEK)}>
                    Week
                </button>
            </div>
            <div className={classes.graphics}>
                <div className={classes.cpu}>
                    <ChartWrapper data={cpuTemp} color={'#7E7E7E'} text={'CPU temperature'} />
                    <ChartWrapper data={gpuTemp} color={'#7E7E7E'} text={'GPU temperature'} />
                </div>
                <div className={classes.gpu}>
                    <ChartWrapper data={cpuLoad} color={'#7E7E7E'} text={'CPU Usage'} />
                    <ChartWrapper data={gpuLoad} color={'#7E7E7E'} text={'GPU Usage'} />
                </div>
                <div className={classes.ram}>
                    <ChartWrapper data={ramTemp} color={'#7E7E7E'} text={'RAM'} />
                </div>
            </div>
        </div>
    )
}

export default Graphics