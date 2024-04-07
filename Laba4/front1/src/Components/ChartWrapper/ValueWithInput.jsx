import classes from '../../pages/Values.module.css'

const ValueWithInput = (props) => {
    return (
        <div className={classes.container}>
            <div className={classes.value_text}>
                {`${props.text}:` + ` ${props.value ?? ''}${props.symbol}`}
            </div>
            <div className={classes.value}>
                {}
            </div>
            { /*<div className={classes.label}>
                Крит.
    </div>*/ }
            <div className={classes.input}>
                <span>Critical: <input type='number' value={props.criticalValue} onChange={(e) => props.setCriticalValue(e.target.value)} /></span>
            </div>
            
        </div>
    )
}

export default ValueWithInput