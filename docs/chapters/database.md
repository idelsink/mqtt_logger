# Database

ER-diagram of the database.

This setup was chosen so that in the future more information can be logged
without to much effort.

\usetikzlibrary{positioning}
\usetikzlibrary{shadows}
\begin{figure}[H]
    \centering
    \resizebox{.8\textwidth}{!}{
        \input{./misc/er_diagram}
    }
    \caption{MQTT Logger entity relation diagram}
    \label{fig:mqtt_logger_entity_relation_diagram}
\end{figure}
