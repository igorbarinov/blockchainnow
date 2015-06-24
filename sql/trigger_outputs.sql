DROP FUNCTION transactions_kafka_procedure_outputs() cascade;

CREATE FUNCTION transactions_kafka_procedure_outputs() RETURNS trigger AS $transactions_kafka_procedure_outputs$
    BEGIN
    	--PERFORM kafka.produce('outputs_', 'debug');
    	PERFORM kafka.produce('outputs', NEW.id || ',' || NEW.hsh || ',' || NEW.amount || ',' || encode(NEW.script, 'hex') || ',' || NEW.position || ',' || NEW.spent || ',' || NEW.type );
		RETURN NEW;
    END;
$transactions_kafka_procedure_outputs$ LANGUAGE plpgsql;

CREATE TRIGGER transactions_kafka_trigger_outputs
AFTER INSERT
ON outputs
FOR EACH ROW
EXECUTE PROCEDURE transactions_kafka_procedure_outputs();
