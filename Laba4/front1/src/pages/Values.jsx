import axios from 'axios';
import { useEffect, useState } from 'react';
import classes from './Values.module.css'
import ValueWithInput from '../Components/ChartWrapper/ValueWithInput';

const Values = () => {

  const sendCriticalValues = () => {
    axios.post('', { CPU_TEMP_CRITICAL: criticalCpuTemp, GPU_TEMP_CRITICAL: criticalGpuTemp, CPU_BUSY_CRITICAL: criticalCpuLoad, GPU_BUSY_CRITICAL: criticalGpuLoad, RAM_BUSY_CRITICAL: criticalRamLoad })
  }

  const [values, setValues] = useState({})

  const [criticalCpuTemp, setCritcialCpuTemp] = useState(null)
  const [criticalGpuTemp, setCritcialGpuTemp] = useState(null)
  const [criticalCpuLoad, setCritcialCpuLoad] = useState(null)
  const [criticalGpuLoad, setCritcialGpuLoad] = useState(null)
  const [criticalRamLoad, setCritcialRamLoad] = useState(null)

  useEffect(() => {
    subscribe()
  }, [values])

  const subscribe = async () => {
    try {
      const { data } = await axios.get('http://192.168.116.54:8000/current_stat')
      setValues(data)
    } catch (e) {
      setTimeout(() => {
        subscribe()
      }, 500)
    }
  }

  return (
    <div className={classes.values}>
      <ValueWithInput text={'RAM'} value={values?.ram_busy} criticalValue={criticalRamLoad} setCriticalValue={setCritcialRamLoad} symbol={'%'} />
      <ValueWithInput text={'CPU temperature'} value={values?.cpu_temp} criticalValue={criticalCpuTemp} setCriticalValue={setCritcialCpuTemp} symbol={'°С'} />
      <ValueWithInput text={'GPU temperature'} value={values?.gpu_temp} criticalValue={criticalGpuTemp} setCriticalValue={setCritcialGpuTemp} symbol={'°С'} />
      <ValueWithInput text={'CPU Usage'} value={values?.cpu_busy} criticalValue={criticalCpuLoad} setCriticalValue={setCritcialCpuLoad} symbol={'%'} />
      <ValueWithInput text={'GPU Usage'} value={values?.gpu_busy} criticalValue={criticalGpuLoad} setCriticalValue={setCritcialGpuLoad} symbol={'%'} />
      <button onClick={sendCriticalValues}>Save</button>
    </div>
  )
}

export default Values